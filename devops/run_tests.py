import os
import subprocess
import config

test_runner = f'{config.Editor_Path} {config.Project_Path} --ExecCmds="Automation RunTest Test_Auto.Science;Quit" -log -abslog={config.Tests_Log_Path} -nosplash -ReportOutputPath={config.Report_Output_Path}'
tests_coverage = f'{config.Open_CPP_Coverage_Path} --modules={config.Project_Root_Path} --sources={config.Source_Path} --excluded_sources={config.Source_Path}\\Automation\\Tests --export_type={config.Coverage_Export_Type}:{config.Coverage_Output_Path} --'

command = f'{tests_coverage} {test_runner}'

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
