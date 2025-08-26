import os

folder_path = os.path.dirname(os.path.abspath(__file__))

for root, dirs, files in os.walk(folder_path):
    for file in files:
        if ".py" not in file:
            print(os.path.relpath(os.path.join(root, file), folder_path))