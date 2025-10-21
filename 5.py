import math
import random

# --- Constantes ---
AMBIENT_INTENSITY = 0.1  # 10% de lumière ambiante partout
epsilon = 0.001          # Petit nombre pour éviter les problèmes de précision

# --- Classe Vector (Inchangée) ---
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

# --- Classes des Objets de la Scène ---

# 1. Le "Contrat" de base
class Hittable:
    def hit(self, ray_origin, ray_direction):
        return -1.0
    def get_normal(self, hit_point):
        return Vector(0, 0, 0)

# 2. La Sphère
class Sphere(Hittable):
    def __init__(self, center, radius, material):
        self.center = center
        self.radius = radius
        self.material = material
    
    def hit(self, ray_origin, ray_direction):
        oc = ray_origin - self.center
        a = ray_direction.dot(ray_direction)
        b = 2.0 * oc.dot(ray_direction)
        c = oc.dot(oc) - self.radius * self.radius
        discriminant = b*b - 4*a*c
        if discriminant < 0:
            return -1.0
        else:
            sqrt_discriminant = math.sqrt(discriminant)
            t1 = (-b - sqrt_discriminant) / (2 * a)
            t2 = (-b + sqrt_discriminant) / (2 * a)
            if t1 > epsilon: # On utilise epsilon au lieu de 0
                if t2 > epsilon: return min(t1, t2)
                else: return t1
            elif t2 > epsilon:
                return t2
            return -1.0
            
    def get_normal(self, hit_point):
        return (hit_point - self.center).normalize()

# 3. Le Plan
class Plane(Hittable):
    def __init__(self, anchor, normal, material):
        self.anchor = anchor
        self.normal = normal
        self.material = material
    
    def hit(self, ray_origin, ray_direction):
        denominator = ray_direction.dot(self.normal)
        if abs(denominator) > epsilon:
            numerator = (self.anchor - ray_origin).dot(self.normal)
            t = numerator / denominator
            if t > epsilon:
                return t
        return -1.0
        
    def get_normal(self, hit_point):
        return self.normal

# --- Configuration de la Scène ---

# Image
image_width = 1920
image_height = 1080
samples_per_pixel = 100

# Scène
camera_origin = Vector(0, 0, 0)
light_position = Vector(-5, 5, 0)

# Matériaux
mat_sphere = {'color': (255, 0, 0)}
mat_plane = {'color': (150, 150, 150)}
mat_sphere_2 = {'color': (0, 255, 0)} # Ajoutons une autre sphère !

# Liste des objets
scene_objects = [
    Sphere(Vector(0, 0, -3), 1.0, mat_sphere),
    Plane(Vector(0, -1, 0), Vector(0, 1, 0), mat_plane),
    Sphere(Vector(-2.5, 0, -3), 0.5, mat_sphere_2) # Une petite sphère verte !
]

# Couleurs
color_sky = (135, 206, 235)

# --- Rendu ---

with open("scene_final.ppm", "w") as f:
    f.write("P3\n")
    f.write(f"{image_width} {image_height}\n")
    f.write("255\n")

    for y in range(image_height - 1, -1, -1):
        print(f"Lignes restantes: {y}", end='\r')
        for x in range(image_width):
            
            total_color_r = 0
            total_color_g = 0
            total_color_b = 0

            for s in range(samples_per_pixel):
                
                u = (x + random.random() - image_width / 2) / (image_width / 2)
                v = (y + random.random() - image_height / 2) / (image_height / 2)
                aspect_ratio = image_width / image_height
                u *= aspect_ratio
                
                ray_direction = Vector(u, v, -1).normalize()

                # 1. Trouver l'objet le plus proche
                t_min = float('inf')
                closest_hit_object = None
                for obj in scene_objects:
                    t = obj.hit(camera_origin, ray_direction)
                    if t > 0 and t < t_min:
                        t_min = t
                        closest_hit_object = obj

                # 2. Si on a touché quelque chose, calculer la couleur
                if closest_hit_object is not None:
                    hit_point = camera_origin + (ray_direction * t_min)
                    normal = closest_hit_object.get_normal(hit_point)
                    base_color = closest_hit_object.material['color']

                    # 3. Test d'ombre
                    is_in_shadow = False
                    shadow_ray_dir = light_position - hit_point
                    shadow_ray_origin = hit_point + (normal * epsilon)
                    
                    for obj in scene_objects:
                        if obj.hit(shadow_ray_origin, shadow_ray_dir) > epsilon:
                            is_in_shadow = True
                            break

                    # 4. Calcul final de la couleur (Ambiant + Diffus)
                    if is_in_shadow:
                        r = base_color[0] * AMBIENT_INTENSITY
                        g = base_color[1] * AMBIENT_INTENSITY
                        b = base_color[2] * AMBIENT_INTENSITY
                    else:
                        L = (light_position - hit_point).normalize()
                        brightness = max(0, normal.dot(L))
                        
                        r = (base_color[0] * AMBIENT_INTENSITY) + (base_color[0] * (1.0 - AMBIENT_INTENSITY) * brightness)
                        g = (base_color[1] * AMBIENT_INTENSITY) + (base_color[1] * (1.0 - AMBIENT_INTENSITY) * brightness)
                        b = (base_color[2] * AMBIENT_INTENSITY) + (base_color[2] * (1.0 - AMBIENT_INTENSITY) * brightness)
                        
                    color_sample = (r, g, b)
                else:
                    color_sample = color_sky
                
                total_color_r += color_sample[0]
                total_color_g += color_sample[1]
                total_color_b += color_sample[2]
            
            # Faire la moyenne
            avg_r = total_color_r / samples_per_pixel
            avg_g = total_color_g / samples_per_pixel
            avg_b = total_color_g / samples_per_pixel # Petite typo corrigée ici -> avg_b
            
            f.write(f"{int(avg_r)} {int(avg_g)} {int(avg_b)}\n")

print("\nImage 'scene_final.ppm' générée !")
