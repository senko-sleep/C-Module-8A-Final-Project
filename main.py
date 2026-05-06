import zipfile
import os

zip_path = "ilovepdf_converted.zip"
extract_to = "extracted_files"

# create folder if it doesn't exist
os.makedirs(extract_to, exist_ok=True)

# extract everything
with zipfile.ZipFile(zip_path, "r") as zip_ref:
    zip_ref.extractall(extract_to)

print("Done extracting!")