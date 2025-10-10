# Image dimensions
image_width = 400
image_height = 200

# Open a file to write to
with open("gradient.ppm", "w") as f:
    # Write the PPM header
    f.write("P3\n")
    f.write(f"{image_width} {image_height}\n")
    f.write("255\n")

    # Loop through every pixel
    for y in range(image_height):
        for x in range(image_width):
            # 1. Calcule la couleur du pixel (x, y)
            r = int((x / image_width) * 255)
            g = int((y / image_height) * 255)
            b = 0
            
            # 2. Écris cette couleur dans le fichier
            f.write(f"{r} {g} {b}\n")

print("Image 'gradient.ppm' créée !")