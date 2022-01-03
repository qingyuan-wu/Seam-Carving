from PIL import Image
import os
import sys
import shutil

def write_image(image, filename):
    height = image.height
    width = image.width

    f = open(filename, "wb")

    f.write(height.to_bytes(2, byteorder='big'))
    f.write(width.to_bytes(2, byteorder='big'))
    img_raster = []
    for i in range(height):
        for j in range(width):
            img_raster.extend(image.getpixel((j, i)))

    f.write(bytearray(img_raster))
    f.close()


def read_2bytes(f):
    bytes = f.read(2) # [int(f.read(1)), int(f.read(1))]
    return int.from_bytes(bytes, byteorder = 'big')


def read_image(filename):
    f = open(filename, "rb")
    height = read_2bytes(f)
    width = read_2bytes(f)
    image = Image.new("RGB", (width, height))
    bytes = f.read()
    for i in range(height):
        for j in range(width):
            image.putpixel((j, i), (bytes[3*(i*width + j)+0],
                                    bytes[3*(i*width + j)+1],
                                    bytes[3*(i*width + j)+2]))

    return image


def set_up_folders(base_path):
    for folder in ["./bins", "./energies", "./paths", "./seams", "./outs"]:
        path = os.path.abspath(os.path.join(base_path, folder))
        if os.path.exists(path):
            shutil.rmtree(path)
        os.makedirs(path)


def convert_folder(base_path, from_dir, to_dir):
    from_dir = os.path.join(base_path, from_dir)
    to_dir = os.path.join(base_path, to_dir)
    to_convert = [file for file in os.listdir(from_dir) if (".png" in file or ".jpg" in file or ".bmp" in file or ".jpeg" in file) and "out" not in file]
    in_paths = [os.path.join(from_dir, filename) for filename in to_convert]
    out_paths = [os.path.join(to_dir, "test-%05d.bin" % i) for i in range(len(to_convert))]
    for in_path, out_path in zip(in_paths, out_paths):
        im = Image.open(in_path)
        write_image(im, out_path)


def recover_tests(from_dir):
    to_convert = [file for file in os.listdir(from_dir) if ".bin" in file]
    in_paths = [os.path.join(from_dir, filename) for filename in to_convert]
    out_paths = [os.path.splitext(path)[0] + '.png' for path in in_paths]
    for in_path, out_path in zip(in_paths, out_paths):
        im = read_image(in_path)
        im.save(out_path)

# if __name__ == "__main__":
#     base_path = os.path.dirname(sys.argv[0])

#     set_up_folders(base_path)
#     convert_folder(base_path, "./in", "./bins")
#     # recover_tests("./outs")

### Write a png image to bin
# image = Image.open("peyto.png")
# write_image(image, "peyto.bin")

### Write multiple images from bin to png
for i in range(55):
    image = read_image("Carved-Images\\img%d.bin" % i)
    image.save("Carved-Images\\img%d.png" % i)
    print(f"i = {i}")