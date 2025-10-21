import math

# --- Notre classe pour manipuler les vecteurs ---
class Vector:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
    
    def dot(self, other):
        return self.x * other.x + self.y * other.y + self.z * other.z
    
    def __sub__(self, other):
        return Vector(self.x - other.x, self.y - other.y, self.z - other.z)

# --- La fonction qui vérifie l'intersection ---
def hit_sphere(center, radius, ray_origin, ray_direction):
    oc = ray_origin - center
    a = ray_direction.dot(ray_direction)
    b = 2.0 * oc.dot(ray_direction)
    c = oc.dot(oc) - radius * radius
    discriminant = b*b - 4*a*c
    return discriminant >= 0

# --- Programme principal ---
# Image
image_width = 400
image_height = 200

# Scène
camera_origin = Vector(0, 0, 0)
sphere_center = Vector(0, 0, -1)
sphere_radius = 0.5

# Rendu
with open("sphere.ppm", "w") as f:
    f.write("P3\n")
    f.write(f"{image_width} {image_height}\n")
    f.write("255\n")

    for y in range(image_height):
        for x in range(image_width):
            # Normaliser les coordonnées du pixel entre -1 et 1
            u = (x / image_width) * 2.0 - 1.0
            v = (y / image_height) * 2.0 - 1.0
            
            # Ajuster pour l'aspect ratio
            aspect_ratio = image_width / image_height
            u *= aspect_ratio
            
            ray_direction = Vector(u, v, -1)
            
            if hit_sphere(sphere_center, sphere_radius, camera_origin, ray_direction):
                color = (255, 0, 0)  # Rouge si on touche la sphère
            else:
                color = (135, 206, 235) # Bleu ciel sinon
            
            f.write(f"{color[0]} {color[1]} {color[2]}\n")

print("Image 'sphere.ppm' générée !")