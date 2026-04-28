#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""!
@file setup.py
@brief Python script to download dependencies, runs only when make command is ran from "ivs-project-2/makefile"
@author Ha Pham 
@date 6.4.2026
"""

import os
import subprocess
import platform

project_dir = os.path.dirname(os.path.abspath(os.path.dirname(os.path.abspath(__file__)))) #Gets project_dir by project_dir/src/setup.py
venv_dir_name = "env"
os_name = platform.system()
requirements_absolute_path = os.path.join(os.path.dirname(__file__), "requirements.txt")

print("Starting setup script")

if os_name == "Linux":
    
    # Ubuntu dependencies
    with open(os.path.join(os.path.dirname(__file__), "ubuntu_dependencies.txt"), "r") as f:
        install_line = f.readline().split()
    subprocess.run(install_line, check=True)

    # If venv does not exist
    if not os.path.isdir(os.path.join(project_dir,venv_dir_name)):
        subprocess.run(["python3", "-m", "venv", f"{project_dir}/{venv_dir_name}"], check=True)
    subprocess.run([f"{project_dir}/{venv_dir_name}/bin/pip", "install", "-r", f"{requirements_absolute_path}"], check=True)

elif os_name == "Darwin":
    if not os.path.isdir(os.path.join(project_dir,venv_dir_name)):
        subprocess.run(["python3", "-m", "venv", f"{project_dir}/{venv_dir_name}"], check=True)
    subprocess.run([f"{project_dir}/{venv_dir_name}/bin/pip", "install", "-r", f"{requirements_absolute_path}"], check=True)
else:
    # If venv does not exist
    if not os.path.isdir(os.path.join(project_dir,venv_dir_name)):
        subprocess.run(["python", "-m", "venv", f"{project_dir}\\{venv_dir_name}"], check=True)
    subprocess.run([f"{project_dir}\\{venv_dir_name}\\Scripts\\pip", "install", "-r", f"{requirements_absolute_path}"], check=True)

print("Setup successful!")