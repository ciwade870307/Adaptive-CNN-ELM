# 2020 NTU MSoC Final Project - Trainable Lenet5-ELM

<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
* [Enviroments](#enviroments)
* [Usage](#usage)
* [Algorithm](#algorithm)
* [References](#references)
* [Contributing](#contributing)
* [Contact](#contact)

<!-- ABOUT THE PROJECT -->
## About The Project
This is a template github repository for hls projects with credits >3.

**Directory structure**
* **README.md** - introduce the project, algorithm, reference ....
* **main.cpp** - Testbench used in this project
* **script.tcl** - script for run this project (Detail will be shown below)
* **MNIST_DATA/** - Normalized features and labels seperated into training and testing dataset
* **Param/** - Pre-trained parameters of CNN and ELM in software

* **src/** - HLS codes
  * **inference/** - Codes related to the inference including CNN-based feature extractor and ELM inference
  * **train/** - Codes related to the training including QRD-ELM (This part research is undergoing, so will be releaesed later)
  * Other codes are used in testbench

* **ipy_Host/** - System-level implementation
  * The jupyter notebook code to control the system between host and IP
  * The bitstream of the IP 
  
## Enviroments
Zedboard(Xynp 7z020) is used in this project
      
<!-- USAGE EXAMPLES -->
## Usage
* **Preparation**
	* Step1: Download the dataset from this link: https://drive.google.com/file/d/1uuevUSQEew2KiXiNsO_NxAr4zXbopWC6/view?usp=sharing
	* Step2: Upzip the Dataset.zip
	* Step3: Copy the all files to the **MNIST_DATA/** file
	
* **Reproduce the HLS flow**
	* Open the Vivado HLS command Prompt, and enter the following command
	```sh
	vivado_hls -f script.tcl 
	```
	
* **Host Program**
  * Step1: Copy all the files in **ipy_Host/** to the board
  * Step2: Open the "proposed_model.ipynb" to run the program

<!-- Algorithm -->
## Algorithm

<!-- References -->
## References
source code from: https://github.com/changwoolee/lenet5_hls

<!-- CONTRIBUTING -->
## Contributing
* Chi-Wei Chen (National Taiwan University, Taipei, Taiwan)
* Max (National Taiwan University, Taipei, Taiwan)
* Lisheng (National Taiwan University, Taipei, Taiwan)

<!-- CONTACT -->
## Contact
* Chi-Wei Chen (National Taiwan University, Taipei, Taiwan) r09943008@ntu.edu.tw



