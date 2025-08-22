import os
import subprocess
import config

command = f'{config.Editor_Path} {config.Project_Path} --ExecCmds="Automation RunTest Test_Auto.Science;Quit" -log -abslog={config.Tests_Log_Path} -nosplash -ReportOutputPath={config.Report_Output_Path}'

subprocess.run(command, shell=True)

# Copy tests artefacts
content_copy_cmd = f'robocopy {config.Tests_Data_Path} {config.Report_Output_Path} /E'
subprocess.run(content_copy_cmd, shell=True)

# Start local server
local_host = f'http://localhost:{config.Server_Port}'
open_page_cmd = f'start "" "{local_host}"'
subprocess.run(open_page_cmd, shell=True)

local_server_start_cmd = f'npx http-server -p {config.Server_Port}'
subprocess.run(local_server_start_cmd, shell=True, check=True, cwd=config.Report_Output_Path)
