#include "my_cordic.h"
#include "PE.h"

#include "../common.h"
void boundary_cell(cos_sin_type x_in,cos_sin_type r_in, mode_type QRD, cos_sin_type& r_out, theta_type& sita_out)
{
	cos_sin_type _;
	if(QRD)
	{
		if(x_in == 0.0)
		{
			sita_out = 0.0;
			r_out = r_in;
			return;
		}
		else if(r_in == 0.0)
		{
			sita_out = 1.5707963;
			r_out = x_in;
			return;
		}
	}
	cordic(bool(1),QRD,r_in,x_in,0.0,r_out,_,sita_out);
}

void internal_cell(cos_sin_type x_in, cos_sin_type y_in, theta_type z_in, mode_type QRD, cos_sin_type& x_out, cos_sin_type& y_out, theta_type& z_out)
{
	cos_sin_type z_in_neg = -1*(z_in);
	theta_type _;
	if(QRD)
	{
		if(z_in == 0.0)
		{
			x_out = x_in;
			y_out = y_in;
			z_out = z_in;
			return;
		}
	}
	z_out = z_in;
	cordic(bool(0),QRD,x_in,y_in,z_in_neg,x_out,y_out,_);
}
