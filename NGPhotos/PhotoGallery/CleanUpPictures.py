import os
import glob

def delete_files():
    # Files from path /mnt/c/Users/williamsm/Documents/personal_workspace/novagenesis/Scripts/Python/
    jpg_files = glob.glob('/mnt/c/Users/williamsm/Documents/personal_workspace/novagenesis/Scripts/Python/*.jpg')
    # Keep only the last 10 files
    jpg_files = sorted(jpg_files, key=os.path.getmtime)
    jpg_files = jpg_files[:-10]
    for file in jpg_files:
        os.remove(file)

delete_files()
print("Done")
