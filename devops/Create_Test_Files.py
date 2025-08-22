import os

def create_files():
    while True:

        while True:
            project_name = input("Enter project name: ")

            if (project_name.strip() != ""):
                break

        while True:
            class_name = input("Enter new class name: ")

            if (class_name.strip() != ""):
                break

        path_in_source = input("Enter path in Source, use '\\' symbol for subdirs: ")

        header_file = f"{class_name}.h"
        source_file = f"{class_name}.cpp"

        header_content = f"""#pragma once"""

        source_content = f"""#include "{project_name}/{path_in_source}/{class_name}.h"
#include "Misc/AutomationTest.h" 

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTest_Class_Name, "Category.Test_Name", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)

// FTest_Class_Name
//------------------------------------------------------------------------------------------------------------
bool FTest_Class_Name::RunTest(const FString &parameters)
{{
	return true;
}}
//------------------------------------------------------------------------------------------------------------
"""
        script_dir = os.path.dirname(os.path.abspath(__file__) )
        project_dir = os.path.join(script_dir, f"..\\Source\\{project_name}\\{path_in_source}")

        if not  os.path.exists(project_dir):
            print(f"Error path {project_dir} not found")
            return

        header_file_path = os.path.join(project_dir, header_file)
        source_file_path = os.path.join(project_dir, source_file)

        print("==========Files to be generated==========")
        print(header_file_path)
        print(source_file_path)
        print("=========================================")

        is_scip_iteration = False

        while True:
            is_confirm_str = input("Confirm? Y/N or (E)xit:")

            if (is_confirm_str.upper == "N"):
                is_scip_iteration = True
                break

            elif (is_confirm_str.upper() == "E"):
                return
            
            elif (is_confirm_str.upper() == "Y"):
                with open(header_file_path, 'w') as f:
                    f.write(header_content)

                with open(source_file_path, 'w') as f:
                    f.write(source_content)

                print("==========Files was generated==========")
                break

        if (is_scip_iteration):
            continue

if __name__ == "__main__":
    create_files()