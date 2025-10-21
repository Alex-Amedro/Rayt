import math
import random

# --- Constantes ---
epsilon = 0.001          # Petit nombre pour éviter les problèmes de précision
MAX_DEPTH = 50           # Nombre maximum de rebonds par rayon

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

# --- Fonctions Utilitaires ---

def random_in_unit_sphere():
    while True:
        p = Vector(random.random() * 2 - 1, random.random() * 2 - 1, random.random() * 2 - 1)
        if p.length_squared() < 1:
            return p

def random_unit_vector():
    p = random_in_unit_sphere()
    return p.normalize()

def reflect(v, n):
    return v - (n * (2 * v.dot(n)))

def refract(v_in, n, ior_ratio):
    cos_theta = min(-v_in.dot(n), 1.0)
    r_out_perp = (v_in + (n * cos_theta)) * ior_ratio
    r_out_parallel_sq = 1.0 - r_out_perp.length_squared()
    if r_out_parallel_sq > 0:
        r_out_parallel = n * -math.sqrt(r_out_parallel_sq)
        return r_out_perp + r_out_parallel
    else:
        return None

def reflectance(cosine, ref_idx):
    r0 = (1 - ref_idx) / (1 + ref_idx)
    r0 = r0 * r0
    return r0 + (1 - r0) * math.pow((1 - cosine), 5)

# --- Classes des Objets de la Scène (Inchangées) ---
class Hittable:
    def hit(self, ray_origin, ray_direction): return -1.0
    def get_normal(self, hit_point): return Vector(0, 0, 0)

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
        if discriminant < 0: return -1.0
        else:
            sqrt_discriminant = math.sqrt(discriminant)
            t1 = (-b - sqrt_discriminant) / (2 * a)
            t2 = (-b + sqrt_discriminant) / (2 * a)
            if t1 > epsilon:
                if t2 > epsilon: return min(t1, t2)
                else: return t1
            elif t2 > epsilon: return t2
            return -1.0
    def get_normal(self, hit_point):
        return (hit_point - self.center).normalize()

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
            if t > epsilon: return t
        return -1.0
    def get_normal(self, hit_point): return self.normal

# --- CŒUR DU MOTEUR : La Fonction Récursive `ray_color` ---
def ray_color(ray_origin, ray_direction, depth):
    
    if depth <= 0:
        return (0, 0, 0)

    # 1. Trouver l'objet le plus proche
    t_min = float('inf')
    closest_hit_object = None
    for obj in scene_objects:
        t = obj.hit(ray_origin, ray_direction)
        if t > 0 and t < t_min:
            t_min = t
            closest_hit_object = obj

    # 2. Si on a touché quelque chose...
    if closest_hit_object is not None:
        hit_point = ray_origin + (ray_direction * t_min)
        normal = closest_hit_object.get_normal(hit_point)
        material = closest_hit_object.material
        V_in = ray_direction.normalize()
        base_color = material['color']

        # CAS 1: 'diffuse' (mat) - NOUVELLE LOGIQUE DE REBOND
        if material['type'] == 'diffuse':
            scatter_direction = (normal + random_unit_vector()).normalize()
            # Gérer le cas où le rayon aléatoire est "annulé"
            if scatter_direction.length_squared() < epsilon * epsilon:
                scatter_direction = normal
            
            new_origin = hit_point + (normal * epsilon)
            color_from_scatter = ray_color(new_origin, scatter_direction, depth - 1)
            
            # Atténuation (l'objet absorbe la lumière)
            r = (base_color[0] / 255) * color_from_scatter[0]
            g = (base_color[1] / 255) * color_from_scatter[1]
            b = (base_color[2] / 255) * color_from_scatter[2]
            return (r, g, b)

        # CAS 2: 'metal' (miroir / brossé)
        if material['type'] == 'metal':
            reflected_direction = reflect(V_in, normal)
            roughness = material.get('roughness', 0.0)
            fuzz = random_in_unit_sphere() * roughness
            fuzzy_direction = (reflected_direction + fuzz).normalize()
            
            if fuzzy_direction.dot(normal) > 0:
                new_origin = hit_point + (normal * epsilon)
                color_from_reflection = ray_color(new_origin, fuzzy_direction, depth - 1)
                
                r = (base_color[0] / 255) * color_from_reflection[0]
                g = (base_color[1] / 255) * color_from_reflection[1]
                b = (base_color[2] / 255) * color_from_reflection[2]
                return (r, g, b)
            else:
                return (0, 0, 0)
        
        # CAS 3: 'dielectric' (verre)
        if material['type'] == 'dielectric':
            ior = material['ior']
            front_face = V_in.dot(normal) < 0
            
            if front_face:
                ior_ratio = 1.0 / ior
                n = normal
                cos_theta = min(-V_in.dot(n), 1.0)
            else:
                ior_ratio = ior / 1.0
                n = normal * -1
                cos_theta = min(-V_in.dot(n), 1.0)
            
            reflect_chance = reflectance(cos_theta, ior_ratio)
            refracted_direction = refract(V_in, n, ior_ratio)
            
            if refracted_direction is None or reflect_chance > random.random():
                reflected_direction = reflect(V_in, n)
                new_origin = hit_point + (n * -epsilon)
                color_from_reflection = ray_color(new_origin, reflected_direction, depth - 1)
                return color_from_reflection # Le verre pur ne teinte pas (d'où l'absence de multiplication)
            else:
                new_origin = hit_point + (n * -epsilon)
                color_from_refraction = ray_color(new_origin, refracted_direction, depth - 1)
                return color_from_refraction # Le verre pur ne teinte pas
            
    # 3. Si on n'a rien touché, le CIEL émet de la lumière
    # Créons un joli dégradé pour le ciel
    unit_direction = ray_direction.normalize()
    t_sky = 0.5 * (unit_direction.y + 1.0) # t_sky va de 0.0 (bas) à 1.0 (haut)
    
    color_white = (255, 255, 255)
    color_blue = (135, 206, 235)
    
    r = (1.0 - t_sky) * color_white[0] + t_sky * color_blue[0]
    g = (1.0 - t_sky) * color_white[1] + t_sky * color_blue[1]
    b = (1.0 - t_sky) * color_white[2] + t_sky * color_blue[2]
    
    return (r, g, b)

# --- Configuration de la Scène ---
image_width = 400
image_height = 200
samples_per_pixel = 100

camera_origin = Vector(0, 0, 0)

# Matériaux
mat_ground = {'type': 'diffuse', 'color': (128, 128, 128)} # Sol gris
mat_center = {'type': 'dielectric', 'color': (255, 255, 255), 'ior': 1.5} # Sphère de verre
mat_left = {'type': 'diffuse', 'color': (255, 0, 0)} # Sphère rouge mat
mat_right = {'type': 'metal', 'color': (200, 150, 50), 'roughness': 0.1} # Or brossé

# Liste des objets
scene_objects = [
    Plane(Vector(0, -1, 0), Vector(0, 1, 0), mat_ground),
    Sphere(Vector(0, 0, -3), 1.0, mat_center),
    Sphere(Vector(-2.5, 0, -3), 1.0, mat_left),
    Sphere(Vector(2.5, 0, -3), 1.0, mat_right),
]

# --- Rendu ---
with open("scene_pathtracing.ppm", "w") as f:
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
                color_sample = ray_color(camera_origin, ray_direction, MAX_DEPTH)
                
                total_color_r += color_sample[0]
                total_color_g += color_sample[1]
                total_color_b += color_sample[2]
            
            # Faire la moyenne
            avg_r = total_color_r / samples_per_pixel
            avg_g = total_color_g / samples_per_pixel
            avg_b = total_color_b / samples_per_pixel
            
            # --- CORRECTION GAMMA ---
            # Nos yeux voient la lumière différemment. On applique une correction "gamma 2.0"
            # (simple racine carrée) pour un look plus réaliste.
            avg_r = math.sqrt(avg_r / 255.0) * 255.0
            avg_g = math.sqrt(avg_g / 255.0) * 255.0
            avg_b = math.sqrt(avg_b / 255.0) * 255.0
            
            f.write(f"{int(avg_r)} {int(avg_g)} {int(avg_b)}\n")

print("\nImage 'scene_pathtracing.ppm' générée !")
