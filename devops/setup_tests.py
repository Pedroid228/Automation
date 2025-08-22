import subprocess
import os
import config

current_dir = os.path.dirname(os.path.abspath(__file__) )
data_path = os.path.join(current_dir, "data")

# Install Bower
install_bower_cmd = 'npm install http-server bower -g'
subprocess.run(install_bower_cmd, shell=True)

# Delete previous directory
prev_dir_del_cmd = f'rmdir /s /q {data_path}'

# Copy automation content
content_copy_cmd = f'robocopy {config.UE_Automation_Content_Path} {data_path} /E'
subprocess.run(content_copy_cmd, shell=True)

# Bower packages install
bower_json_path = os.path.join(data_path, 'bower.json')
bower_install_cmd = f'bower install {bower_json_path}'
subprocess.run(bower_install_cmd, shell=True)
