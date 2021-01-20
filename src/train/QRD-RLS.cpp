#include "PE.h"
#include "QRD-RLS.h"

#include <iostream>
using namespace std;

#include "../common.h"

void RELU(float_type& x)
{
	if(x<0.0) x = 0.0;
}

void QRD_RLS(const float_type x_train[N_INPUT_NODES],const int t_train,const float_type wmem[N_INPUT_NODES*N_HIDDEN_NODES],const float_type bmem[N_HIDDEN_NODES],const bool BS,float_type beta_mem[N_HIDDEN_NODES*N_OUTPUT_NODES])
{
	float_type H_train[N_HIDDEN_NODES] = {0.0};
	float_type T_train[N_OUTPUT_NODES] = {0.0};
	static float_type R_mem[int(((N_HIDDEN_NODES + 1)*N_HIDDEN_NODES)/2)] = {0.0};
	static float_type u_mem[N_HIDDEN_NODES * N_OUTPUT_NODES] = {0.0};
	float_type ru_in_buf[1+NUM_OF_IC] = {0.0};
	float_type ru_out_buf[1+NUM_OF_IC] = {0.0};
	float_type x_in[1+NUM_OF_IC] = {0.0};
	float_type t_in[1+NUM_OF_IC] = {0.0};
	float_type x_out[1+NUM_OF_IC] = {0.0};
	float_type t_out[1+NUM_OF_IC] = {0.0};

	int num_of_cell = 1;
	int num_of_u_cell = 1;
	int r_in_cnt = 0;
	int r_out_cnt = 0;
	int u_addr = 0;
	int u_out_addr = 0;
	CALC_H_LOOP:
		for(int i = 0;i < N_HIDDEN_NODES;i++)
		{
			for(int j = 0; j < N_INPUT_NODES;j++)
			{
				H_train[i] = H_train[i] + wmem[i*N_INPUT_NODES + j]*x_train[j];
			}
			H_train[i] = H_train[i] + bmem[i];
			RELU(H_train[i]);
		}
	CALC_T_LOOP:
		for(int i = 0;i < N_OUTPUT_NODES; i++)
		{
			if(i == t_train)
				T_train[i] = 1.0;
			else
				T_train[i] = -1.0;
		}
	if(BS == false)
	{
		u_addr = 0;
		r_in_cnt = 0;
		r_out_cnt = 0;
		QRD_RLS_LOOP:
			for(int i = 0;i<QRD_RLS_NEED_CNT;i++)
			{
				/*============For ru_in_buf==============*/
				if(i<N_HIDDEN_NODES)
				{
					for(int j = 0;j < num_of_cell;j++)
					{
						if(i%2 == 0)
							ru_in_buf[j] = R_mem[r_in_cnt];
						else
							ru_in_buf[j+1] = R_mem[r_in_cnt];
						r_in_cnt++;
					}
				}
				else if(i<2*N_HIDDEN_NODES-1)
				{
					for(int j = 0;j < num_of_cell - num_of_u_cell;j++)
					{
						if(i%2 == 0)
							ru_in_buf[j] = R_mem[r_in_cnt];
						else
							ru_in_buf[j+1] = R_mem[r_in_cnt];
						r_in_cnt++;
					}
					for(int k = 0;k < num_of_u_cell;k++)
					{
						if(i%2 == 0)
							ru_in_buf[num_of_cell - num_of_u_cell + k] = u_mem[u_addr + k*(N_HIDDEN_NODES - 1)];
						else
							ru_in_buf[num_of_cell - num_of_u_cell +k + 1] = u_mem[u_addr + k*(N_HIDDEN_NODES  - 1)];
					}
					u_addr++;
				}
				else
				{
					for(int j = 0;j < num_of_cell;j++)
					{
						ru_in_buf[j+1] = u_mem[u_addr + j*(N_HIDDEN_NODES - 1)];
					}
					u_addr = u_addr + N_HIDDEN_NODES;
				}
				/*============================================*/
				/*=====================For PE================*/
				if(i<N_HIDDEN_NODES)
				{
					if(i%2 == 0)
						x_in[num_of_cell - 1] = H_train[i];
					else
						x_in[num_of_cell] = H_train[i];
				}
				else if(i<N_HIDDEN_NODES + N_OUTPUT_NODES)
				{
					if(i%2 == 0)
						x_in[num_of_cell - 1] = T_train[i - N_HIDDEN_NODES];
					else
						x_in[num_of_cell] = T_train[i - N_HIDDEN_NODES];
				}

				if(i!=0 && i%2 == 0)
					x_in[0] = x_out[0];
				for(int j = NUM_OF_IC;j >0;j--)
				{
					if(i%2 == 0 || i > 2*N_HIDDEN_NODES - 1)
					{
						if(j!= num_of_cell-1 || i >= N_HIDDEN_NODES + N_OUTPUT_NODES)
							x_in[j] = x_out[j];
						t_in[j-1] = t_out[j];
					}
					else
					{
						if(j!= num_of_cell || i >= N_HIDDEN_NODES + N_OUTPUT_NODES)
							x_in[j] = x_out[j-1];
						t_in[j-1] = t_out[j-1];
					}
				}
				Internal_cell_Loop0:
				for(int j = NUM_OF_IC;j>0;j--)
				{
					internal_cell(ru_in_buf[j],x_in[j],t_in[j-1],1,ru_out_buf[j],x_out[j-1],t_out[j]);
				}
				boundary_cell(x_in[0],ru_in_buf[0],1,ru_out_buf[0],t_out[0]);
				/*===================================================*/
				/*======================For Mem======================*/
				if(i<N_HIDDEN_NODES)
				{
					for(int j = 0;j < num_of_cell;j++)
					{
						if(i%2 == 0)
							R_mem[r_out_cnt] = ru_out_buf[j];
						else
							R_mem[r_out_cnt] = ru_out_buf[j+1];
						r_out_cnt++;
					}
				}
				else if(i<2*N_HIDDEN_NODES - 1)
				{
					for(int j = 0;j < num_of_cell - num_of_u_cell;j++)
					{
						if(i%2 == 0)
							R_mem[r_out_cnt] = ru_out_buf[j];
						else
							R_mem[r_out_cnt] = ru_out_buf[j+1];
						r_out_cnt++;
					}
					for(int k = 0;k < num_of_u_cell;k++)
					{
						if(i%2 == 0)
							u_mem[u_out_addr + k*(N_HIDDEN_NODES - 1)] = ru_out_buf[k + num_of_cell - num_of_u_cell];
						else
							u_mem[u_out_addr + k*(N_HIDDEN_NODES - 1)] = ru_out_buf[k + num_of_cell - num_of_u_cell + 1];
					}
					u_out_addr++;
				}
				else
				{
					for(int j =0;j < num_of_cell; j++)
						u_mem[u_out_addr+j*(N_HIDDEN_NODES - 1)] = ru_out_buf[j+1];
					u_out_addr = u_out_addr + N_HIDDEN_NODES;
				}
				if(num_of_u_cell < N_OUTPUT_NODES && i >= N_HIDDEN_NODES && i < 2*N_HIDDEN_NODES - 1)
					num_of_u_cell++;
				/*===================================================*/
				/*=====================num_of_cell===================*/
				if(i >= N_HIDDEN_NODES+N_OUTPUT_NODES - 1)
				{
					if(i >= 2*N_HIDDEN_NODES - 1)
						num_of_cell --;
					else if(i%2 == 0)
						num_of_cell --;
				}
				else if(i%2 == 1)
					num_of_cell++;
				/*===================================================*/

			}
			beta_mem[0] = R_mem[0];
	}
	else
	{
		BS_LOOP:
			u_addr = N_HIDDEN_NODES*N_OUTPUT_NODES - 1;
			int len_of_r_mem = int(((N_HIDDEN_NODES + 1)*N_HIDDEN_NODES)/2);
			int beta_out_cnt;
			for(int op = 0;op < N_OUTPUT_NODES;op++)
			{	num_of_cell = 1;
				r_in_cnt = len_of_r_mem - 1;
				beta_out_cnt = N_HIDDEN_NODES*(N_OUTPUT_NODES - op) - 1;
				for(int i = 0;i<BS_NEED_CNT;i++)
				{
					/*====================For ru_in_buf=================*/
					for(int j = num_of_cell -1;j >=0;j--)
					{
						if(i%2==0)
							ru_in_buf[j] = R_mem[r_in_cnt];
						else
							ru_in_buf[j+1] = R_mem[r_in_cnt];
						r_in_cnt--;
					}
					/*==================================================*/
					/*==================For PE==========================*/
					if(i<N_HIDDEN_NODES)
					{
						if(i%2 == 0)
							x_in[num_of_cell - 1] = u_mem[u_addr];
						else
							x_in[num_of_cell] = u_mem[u_addr];
						u_addr--;
					}
					if(i!=0 && i%2 == 0)
						x_in[0] = x_out[0];
					for(int j = NUM_OF_IC_BS; j > 0;j --)
					{
						if(i%2 == 0)
						{
							if(j!= num_of_cell - 1 || i>=N_HIDDEN_NODES)
								x_in[j] = x_out[j];
							t_in[j-1] = t_out[j];
						}
						else
						{
							if(j!= num_of_cell || i >=N_HIDDEN_NODES)
								x_in[j] = x_out[j-1];
							t_in[j-1] = t_out[j-1];
						}
					}
					Internal_cell_Loop1:
					for(int j = NUM_OF_IC_BS;j>0;j--)
						internal_cell(ru_in_buf[j],x_in[j],t_in[j-1],0,ru_out_buf[j],x_out[j-1],t_out[j]);
					boundary_cell(x_in[0],ru_in_buf[0],0,ru_out_buf[0],t_out[0]);
					/*==================================================*/
					/*========================For beta_mem==============*/
					if(i>=N_HIDDEN_NODES - 1)
					{
						if(i%2 == 0)
							beta_mem[beta_out_cnt] = t_out[num_of_cell - 1];
						else
							beta_mem[beta_out_cnt] = t_out[num_of_cell];
						beta_out_cnt--;
					}
					/*==================================================*/
					/*======================num_of_cell=================*/
					if(i>=N_HIDDEN_NODES - 1 && i%2 == 0)
						num_of_cell--;
					else if(i<N_HIDDEN_NODES - 1 && i%2 == 1)
						num_of_cell++;
					/*==================================================*/
				}
		}
	}

}







void QRD_RLS_test(float_type H_train[N_HIDDEN_NODES],float_type T_train[N_OUTPUT_NODES],bool BS,float_type beta_mem[N_HIDDEN_NODES*N_OUTPUT_NODES])
{

	static float_type R_mem[int(((N_HIDDEN_NODES + 1)*N_HIDDEN_NODES)/2)] = {0.0};
	static float_type u_mem[N_HIDDEN_NODES * N_OUTPUT_NODES] = {0.0};
	float_type ru_in_buf[1+NUM_OF_IC] = {0.0};
	float_type ru_out_buf[1+NUM_OF_IC] = {0.0};
	float_type x_in[1+NUM_OF_IC] = {0.0};
	float_type t_in[1+NUM_OF_IC] = {0.0};
	float_type x_out[1+NUM_OF_IC] = {0.0};
	float_type t_out[1+NUM_OF_IC] = {0.0};
	int num_of_cell = 1;
	int num_of_u_cell = 1;
	int r_in_cnt = 0;
	int r_out_cnt = 0;
	int u_addr = 0;
	int u_out_addr = 0;
	if(BS == false)
	{
		u_addr = 0;
		r_in_cnt = 0;
		r_out_cnt = 0;
		QRD_RLS_LOOP:
			for(int i = 0;i<QRD_RLS_NEED_CNT;i++)
			{
				/*============For ru_in_buf==============*/
				if(i<N_HIDDEN_NODES)
				{
					for(int j = 0;j < num_of_cell;j++)
					{
						if(i%2 == 0)
							ru_in_buf[j] = R_mem[r_in_cnt];
						else
							ru_in_buf[j+1] = R_mem[r_in_cnt];
						r_in_cnt++;
					}
				}
				else if(i<2*N_HIDDEN_NODES-1)
				{
					for(int j = 0;j < num_of_cell - num_of_u_cell;j++)
					{
						if(i%2 == 0)
							ru_in_buf[j] = R_mem[r_in_cnt];
						else
							ru_in_buf[j+1] = R_mem[r_in_cnt];
						r_in_cnt++;
					}
					for(int k = 0;k < num_of_u_cell;k++)
					{
						if(i%2 == 0)
							ru_in_buf[num_of_cell - num_of_u_cell + k] = u_mem[u_addr + k*(N_HIDDEN_NODES - 1)];
						else
							ru_in_buf[num_of_cell - num_of_u_cell +k + 1] = u_mem[u_addr + k*(N_HIDDEN_NODES  - 1)];
					}
					u_addr++;
				}
				else
				{
					for(int j = 0;j < num_of_cell;j++)
					{
						ru_in_buf[j+1] = u_mem[u_addr + j*(N_HIDDEN_NODES - 1)];
					}
					u_addr = u_addr + N_HIDDEN_NODES;
				}
				/*============================================*/
				/*=====================For PE================*/
				if(i<N_HIDDEN_NODES)
				{
					if(i%2 == 0)
						x_in[num_of_cell - 1] = H_train[i];
					else
						x_in[num_of_cell] = H_train[i];
				}
				else if(i<N_HIDDEN_NODES + N_OUTPUT_NODES)
				{
					if(i%2 == 0)
						x_in[num_of_cell - 1] = T_train[i - N_HIDDEN_NODES];
					else
						x_in[num_of_cell] = T_train[i - N_HIDDEN_NODES];
				}

				if(i!=0 && i%2 == 0)
					x_in[0] = x_out[0];
				for(int j = NUM_OF_IC;j >0;j--)
				{
					if(i%2 == 0 || i > 2*N_HIDDEN_NODES - 1)
					{
						if(j!= num_of_cell-1 || i >= N_HIDDEN_NODES + N_OUTPUT_NODES)
							x_in[j] = x_out[j];
						t_in[j-1] = t_out[j];
					}
					else
					{
						if(j!= num_of_cell || i >= N_HIDDEN_NODES + N_OUTPUT_NODES)
							x_in[j] = x_out[j-1];
						t_in[j-1] = t_out[j-1];
					}
				}
				for(int j = NUM_OF_IC;j>0;j--)
				{
					internal_cell(ru_in_buf[j],x_in[j],t_in[j-1],1,ru_out_buf[j],x_out[j-1],t_out[j]);
				}
				boundary_cell(x_in[0],ru_in_buf[0],1,ru_out_buf[0],t_out[0]);
				/*===================================================*/
				/*======================For Mem======================*/
				if(i<N_HIDDEN_NODES)
				{
					for(int j = 0;j < num_of_cell;j++)
					{
						if(i%2 == 0)
							R_mem[r_out_cnt] = ru_out_buf[j];
						else
							R_mem[r_out_cnt] = ru_out_buf[j+1];
						r_out_cnt++;
					}
				}
				else if(i<2*N_HIDDEN_NODES - 1)
				{
					for(int j = 0;j < num_of_cell - num_of_u_cell;j++)
					{
						if(i%2 == 0)
							R_mem[r_out_cnt] = ru_out_buf[j];
						else
							R_mem[r_out_cnt] = ru_out_buf[j+1];
						r_out_cnt++;
					}
					for(int k = 0;k < num_of_u_cell;k++)
					{
						if(i%2 == 0)
							u_mem[u_out_addr + k*(N_HIDDEN_NODES - 1)] = ru_out_buf[k + num_of_cell - num_of_u_cell];
						else
							u_mem[u_out_addr + k*(N_HIDDEN_NODES - 1)] = ru_out_buf[k + num_of_cell - num_of_u_cell + 1];
					}
					u_out_addr++;
				}
				else
				{
					for(int j =0;j < num_of_cell; j++)
						u_mem[u_out_addr+j*(N_HIDDEN_NODES - 1)] = ru_out_buf[j+1];
					u_out_addr = u_out_addr + N_HIDDEN_NODES;
				}
				if(num_of_u_cell < N_OUTPUT_NODES && i >= N_HIDDEN_NODES && i < 2*N_HIDDEN_NODES - 1)
					num_of_u_cell++;
				/*===================================================*/
				/*=====================num_of_cell===================*/
				if(i >= N_HIDDEN_NODES+N_OUTPUT_NODES - 1)
				{
					if(i >= 2*N_HIDDEN_NODES - 1)
						num_of_cell --;
					else if(i%2 == 0)
						num_of_cell --;
				}
				else if(i%2 == 1)
					num_of_cell++;
				/*===================================================*/

			}
	}
	else
	{
		BS_LOOP:
			u_addr = N_HIDDEN_NODES*N_OUTPUT_NODES - 1;
			int len_of_r_mem = int(((N_HIDDEN_NODES + 1)*N_HIDDEN_NODES)/2);
			int beta_out_cnt;
			for(int op = 0;op < N_OUTPUT_NODES;op++)
			{
				r_in_cnt = len_of_r_mem - 1;
				beta_out_cnt = N_HIDDEN_NODES*(N_OUTPUT_NODES - op) - 1;
				num_of_cell = 1;
				for(int i = 0;i<BS_NEED_CNT;i++)
				{
					/*====================For ru_in_buf=================*/
					for(int j = num_of_cell -1;j >=0;j--)
					{
						if(i%2==0)
							ru_in_buf[j] = R_mem[r_in_cnt];
						else
							ru_in_buf[j+1] = R_mem[r_in_cnt];
						r_in_cnt--;
					}
					/*==================================================*/
					/*==================For PE==========================*/
					if(i<N_HIDDEN_NODES)
					{
						if(i%2 == 0)
							x_in[num_of_cell - 1] = u_mem[u_addr];
						else
							x_in[num_of_cell] = u_mem[u_addr];
						u_addr--;
					}
					if(i!=0 && i%2 == 0)
						x_in[0] = x_out[0];
					for(int j = NUM_OF_IC_BS; j > 0;j --)
					{
						if(i%2 == 0)
						{
							if(j!= num_of_cell - 1 || i>=N_HIDDEN_NODES)
								x_in[j] = x_out[j];
							t_in[j-1] = t_out[j];
						}
						else
						{
							if(j!= num_of_cell || i >=N_HIDDEN_NODES)
								x_in[j] = x_out[j-1];
							t_in[j-1] = t_out[j-1];
						}
					}
					for(int j = NUM_OF_IC_BS;j>0;j--)
						internal_cell(ru_in_buf[j],x_in[j],t_in[j-1],0,ru_out_buf[j],x_out[j-1],t_out[j]);
					boundary_cell(x_in[0],ru_in_buf[0],0,ru_out_buf[0],t_out[0]);
					/*==================================================*/
					/*========================For beta_mem==============*/
					if(i>=N_HIDDEN_NODES - 1)
					{
						if(i%2 == 0)
							beta_mem[beta_out_cnt] = t_out[num_of_cell - 1];
						else
							beta_mem[beta_out_cnt] = t_out[num_of_cell];
						beta_out_cnt--;
					}
					/*==================================================*/
					/*======================num_of_cell=================*/
					if(i>=N_HIDDEN_NODES - 1 && i%2 == 0)
						num_of_cell--;
					else if(i<N_HIDDEN_NODES - 1 && i%2 == 1)
						num_of_cell++;
					/*==================================================*/
				}
		}
	}

}
