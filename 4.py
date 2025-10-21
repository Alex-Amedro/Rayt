import math
import random  # NOUVEAU: On importe la bibliothèque pour l'aléatoire

# --- Classe Vector (inchangée) ---
class Vector:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y, self.z + other.z)
    def __sub__(self, other):
        return Vector(self.x - other.x, self.y - other.y, self.z - other.z)
    def __mul__(self, scalar):
        return Vector(self.x * scalar, self.y * scalar, self.z * scalar)
    def dot(self, other):
        return self.x * other.x + self.y * other.y + self.z * other.z
    def length_squared(self):
        return self.dot(self)
    def normalize(self):
        length = math.sqrt(self.length_squared())
        if length == 0: return Vector(0, 0, 0)
        return Vector(self.x / length, self.y / length, self.z / length)

# --- Fonction hit_sphere (inchangée) ---
def hit_sphere(center, radius, ray_origin, ray_direction):
    oc = ray_origin - center
    a = ray_direction.dot(ray_direction)
    b = 2.0 * oc.dot(ray_direction)
    c = oc.dot(oc) - radius * radius
    discriminant = b*b - 4*a*c
    if discriminant < 0:
        return -1.0
    else:
        sqrt_discriminant = math.sqrt(discriminant)
        t1 = (-b - sqrt_discriminant) / (2 * a)
        t2 = (-b + sqrt_discriminant) / (2 * a)
        if t1 > 0:
            if t2 > 0: return min(t1, t2)
            else: return t1
        elif t2 > 0:
            return t2
        return -1.0

# --- Programme principal ---

# Image
image_width = 400
image_height = 200
samples_per_pixel = 100  # NOUVEAU: Le nombre d'échantillons par pixel

# Scène
camera_origin = Vector(0, 0, 0)
sphere_center = Vector(0, 0, -3)
sphere_radius = 1.0
light_position = Vector(-5, 5, 0)

# Couleurs
color_sky = (135, 206, 235)
color_lit_base = (255, 0, 0)    # Couleur de base de la sphère
color_shadow = (0, 0, 0)

epsilon = 0.001

# Rendu
with open("sphere_antialiasing.ppm", "w") as f:
    f.write("P3\n")
    f.write(f"{image_width} {image_height}\n")
    f.write("255\n")

    # On inverse la boucle Y pour commencer par le haut (plus intuitif)
    for y in range(image_height - 1, -1, -1):
        # Affiche la progression dans la console
        print(f"Lignes restantes: {y}", end='\r') 
        
        for x in range(image_width):
            
            # NOUVEAU: On initialise la couleur totale pour ce pixel
            total_color_r = 0
            total_color_g = 0
            total_color_b = 0

            # NOUVEAU: Boucle de sur-échantillonnage
            for s in range(samples_per_pixel):
                
                # Calcule u et v avec un décalage aléatoire
                u = (x + random.random() - image_width / 2) / (image_width / 2)
                v = (y + random.random() - image_height / 2) / (image_height / 2)
                
                aspect_ratio = image_width / image_height
                u *= aspect_ratio
                
                ray_direction = Vector(u, v, -1).normalize()

                t = hit_sphere(sphere_center, sphere_radius, camera_origin, ray_direction)
                
                color_sample = color_sky  # Couleur par défaut (ciel)

                if t > 0:
                    hit_point = camera_origin + (ray_direction * t)
                    shadow_ray_dir = light_position - hit_point
                    shadow_ray_origin = hit_point + (shadow_ray_dir.normalize() * epsilon)
                    t_shadow = hit_sphere(sphere_center, sphere_radius, shadow_ray_origin, shadow_ray_dir)

                    if t_shadow > 0:
                        color_sample = color_shadow
                    else:
                        N = (hit_point - sphere_center).normalize()
                        L = (light_position - hit_point).normalize()
                        brightness = max(0, N.dot(L))
                        
                        r = color_lit_base[0] * brightness
                        g = color_lit_base[1] * brightness
                        b = color_lit_base[2] * brightness
                        color_sample = (r, g, b)
                
                # NOUVEAU: On ajoute la couleur de cet échantillon au total
                total_color_r += color_sample[0]
                total_color_g += color_sample[1]
                total_color_b += color_sample[2]
            
            # NOUVEAU: On fait la moyenne et on l'écrit
            avg_r = total_color_r / samples_per_pixel
            avg_g = total_color_g / samples_per_pixel
            avg_b = total_color_b / samples_per_pixel
            
            f.write(f"{int(avg_r)} {int(avg_g)} {int(avg_b)}\n")

print("\nImage 'sphere_antialiasing.ppm' générée !")
