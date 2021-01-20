# 2020 NTU MSoC Final Project - Trainable Lenet5-ELM

<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
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

* **src/**
  * original - original code from open source if there is 
  * final (use inline pragma) - include both host and kernel code ※Note: host code must do auto-check

* **ipy_Host/** - System-level implementation
  * The jupyter notebook code to control the system between host and IP
  * The bitstream of the IP 
  
      
<!-- USAGE EXAMPLES -->
## Usage
* **build process**
  * For example, if you have a host program
  ```sh
  python FIRN11MAXI.py
  ```
  * If you implemented with vitis on U50
  ```sh
  make run
  ```

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



