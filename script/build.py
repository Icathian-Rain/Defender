import os

dll_path= "src\\DllMain\\Debug\\DllMain.dll"
exe_path= "src\\defender\\build\\bin\\defender.exe"
syringe_path = "src\\syringe\\Debug\\syringe.exe"
test_path = "src\\MFCtest\\Debug\\MFCtest.exe"

target_dir = "test\\"

def copy(path, target_dir):
    os.system("copy %s %s" % (path, target_dir))

def main():
    os.chdir("..")
    copy(dll_path, target_dir)
    copy(exe_path, target_dir)
    copy(syringe_path, target_dir)
    copy(test_path, target_dir)


if __name__ == "__main__":
    main()