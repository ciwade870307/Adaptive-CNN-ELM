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
「Sign language」is a major way that the hearing-impaired communicate with other. However, except for the hearing-impaired there are not many people have learned sign language in their lifetime. Hence, communications between both of them are difficult.
Inspired by this situation, we want to devise a system on FPGA(ZedBoard) that can bridge the communication by a local device.
The system on board can recognize the meaning of the user’s gesture. The user can “retrain” their gestures on board to make the system adapt for the user’s physical habit and let recognition of gestures be more accurate.
Then the prediction of gestures can be downstream for other IPs to be converted to output signals (may be image, text, or sound) and convey the idea of the hearing- impaired user to others. But this part is not included in our project this time.


**Directory structure**
* **README.md** - Introduce the project, algorithm, reference ....
* **main.cpp** - Testbench used in this project
* **script.tcl** - Script for running this project (Detail will be shown below)
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
	* Step2: Upzip the "Dataset.zip"
	* Step3: Copy the all files to the **MNIST_DATA/** file
	
* **Reproduce the HLS flow**
	* Open the Vivado HLS command Prompt, and enter the following command
	```sh
	vivado_hls -f script.tcl 
	```
	
* **Host Program**
  * Step1: Upload all the files in **ipy_Host/** to the board
  * Step2: Open the "proposed_model.ipynb" to run the program

<!-- Algorithm -->
## Algorithm
![Adaptive-CNN-ELM](/Adaptive-CNN-ELM.png "Adaptive-CNN-ELM")

<!-- References -->
## References
Source code from: https://github.com/changwoolee/lenet5_hls

<!-- CONTRIBUTING -->
## Contributing
* Chi-Wei Chen (National Taiwan University, Taipei, Taiwan)
* Max (National Taiwan University, Taipei, Taiwan)
* Lisheng (National Taiwan University, Taipei, Taiwan)

<!-- CONTACT -->
## Contact
* Chi-Wei Chen (National Taiwan University, Taipei, Taiwan) r09943008@ntu.edu.tw



