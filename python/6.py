import math
import random

# --- Constantes ---
AMBIENT_INTENSITY = 0.1  # Lumière ambiante globale
epsilon = 0.001          # Petit nombre pour éviter les problèmes de précision
MAX_DEPTH = 50           # Nombre maximum de rebonds par rayon

# --- Classe Vector ---
# Gère toutes nos opérations 3D (points, directions, couleurs)
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

# Crée un vecteur aléatoire dans une sphère unité (pour le métal brossé)
def random_in_unit_sphere():
    while True:
        p = Vector(random.random() * 2 - 1, random.random() * 2 - 1, random.random() * 2 - 1)
        if p.length_squared() < 1:
            return p

# Calcule la direction du reflet (Loi de la réflexion)
def reflect(v, n):
    return v - (n * (2 * v.dot(n)))

# Calcule la direction réfractée (Loi de Snell)
def refract(v_in, n, ior_ratio):
    cos_theta = min(-v_in.dot(n), 1.0)
    r_out_perp = (v_in + (n * cos_theta)) * ior_ratio
    r_out_parallel_sq = 1.0 - r_out_perp.length_squared()
    
    if r_out_parallel_sq > 0:
        r_out_parallel = n * -math.sqrt(r_out_parallel_sq)
        return r_out_perp + r_out_parallel
    else:
        # Réflexion totale interne
        return None

# Calcule la probabilité de reflet vs réfraction (Équations de Fresnel)
def reflectance(cosine, ref_idx):
    r0 = (1 - ref_idx) / (1 + ref_idx)
    r0 = r0 * r0
    return r0 + (1 - r0) * math.pow((1 - cosine), 5)

# --- Classes des Objets de la Scène ---

# 1. Le "Contrat" de base (Classe abstraite)
class Hittable:
    def hit(self, ray_origin, ray_direction):
        # Renvoie la distance 't' ou -1.0 si raté
        return -1.0
    
    def get_normal(self, hit_point):
        # Renvoie la normale à la surface au point d'impact
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
            
            if t1 > epsilon:
                if t2 > epsilon: return min(t1, t2)
                else: return t1
            elif t2 > epsilon:
                return t2
            return -1.0
            
    def get_normal(self, hit_point):
        return (hit_point - self.center).normalize()

# 3. Le Plan (Sol)
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

# --- CŒUR DU MOTEUR : La Fonction Récursive `ray_color` ---
def ray_color(ray_origin, ray_direction, depth):
    
    # Condition d'arrêt : si on a trop rebondi, on renvoie du noir (lumière absorbée)
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

        # --- Branchement selon le type de matériau ---

        # CAS 1: 'diffuse' (mat)
        if material['type'] == 'diffuse':
            base_color = material['color']
            
            # Test d'ombre
            is_in_shadow = False
            shadow_ray_dir = light_position - hit_point
            shadow_ray_origin = hit_point + (normal * epsilon)
            for obj in scene_objects:
                if obj.hit(shadow_ray_origin, shadow_ray_dir) > epsilon:
                    is_in_shadow = True
                    break
            
            # Calcul de la couleur (Ambiante + Diffuse)
            if is_in_shadow:
                r,g,b = (c * AMBIENT_INTENSITY for c in base_color)
                return (r, g, b)
            else:
                L = (light_position - hit_point).normalize()
                brightness = max(0, normal.dot(L))
                r = (base_color[0] * AMBIENT_INTENSITY) + (base_color[0] * (1.0 - AMBIENT_INTENSITY) * brightness)
                g = (base_color[1] * AMBIENT_INTENSITY) + (base_color[1] * (1.0 - AMBIENT_INTENSITY) * brightness)
                b = (base_color[2] * AMBIENT_INTENSITY) + (base_color[2] * (1.0 - AMBIENT_INTENSITY) * brightness)
                return (r, g, b)

        # CAS 2: 'metal' (miroir / brossé)
        if material['type'] == 'metal':
            reflected_direction = reflect(V_in, normal)
            
            # Gestion du flou (roughness)
            roughness = material.get('roughness', 0.0)
            fuzz = random_in_unit_sphere() * roughness
            fuzzy_direction = reflected_direction + fuzz
            
            # On s'assure que le rayon flou ne part pas "sous" la surface
            if fuzzy_direction.dot(normal) > 0:
                new_origin = hit_point + (normal * epsilon)
                color_from_reflection = ray_color(new_origin, fuzzy_direction, depth - 1)
                
                # Teinter le reflet avec la couleur du métal
                base_color = material['color']
                r = (base_color[0] / 255) * color_from_reflection[0]
                g = (base_color[1] / 255) * color_from_reflection[1]
                b = (base_color[2] / 255) * color_from_reflection[2]
                return (r, g, b)
            else:
                # Le rayon aléatoire a été absorbé
                return (0, 0, 0)
        
        # CAS 3: 'dielectric' (verre)
        if material['type'] == 'dielectric':
            base_color = material['color'] # Teinte du verre
            ior = material['ior']
            
            # Déterminer si on entre ou on sort de l'objet
            front_face = V_in.dot(normal) < 0
            
            if front_face:
                ior_ratio = 1.0 / ior
                n = normal
                cos_theta = min(-V_in.dot(n), 1.0)
            else:
                ior_ratio = ior / 1.0
                n = normal * -1 # Inverser la normale
                cos_theta = min(-V_in.dot(n), 1.0)
            
            # 1. Calculer la probabilité de reflet (Fresnel)
            reflect_chance = reflectance(cos_theta, ior_ratio)
            
            # 2. Calculer la réfraction (Snell)
            refracted_direction = refract(V_in, n, ior_ratio)
            
            # 3. Choisir aléatoirement entre reflet et réfraction
            if refracted_direction is None or reflect_chance > random.random():
                # On reflète
                reflected_direction = reflect(V_in, n)
                new_origin = hit_point + (n * -epsilon) # Décalage
                color_from_reflection = ray_color(new_origin, reflected_direction, depth - 1)
                
                r = (base_color[0] / 255) * color_from_reflection[0]
                g = (base_color[1] / 255) * color_from_reflection[1]
                b = (base_color[2] / 255) * color_from_reflection[2]
                return (r, g, b)
            else:
                # On réfracte
                new_origin = hit_point + (n * -epsilon) # Décalage
                color_from_refraction = ray_color(new_origin, refracted_direction, depth - 1)
                
                r = (base_color[0] / 255) * color_from_refraction[0]
                g = (base_color[1] / 255) * color_from_refraction[1]
                b = (base_color[2] / 255) * color_from_refraction[2]
                return (r, g, b)
            
    # 3. Si on n'a rien touché (rayon parti dans le vide), on renvoie le ciel
    return color_sky

# --- Configuration de la Scène ---
# C'est ici que tu peux jouer !
image_width = 400
image_height = 200
samples_per_pixel = 100 # Qualité de l'anti-aliasing (augmente le temps de calcul)

camera_origin = Vector(0, 0, 0)
light_position = Vector(-5, 5, 0)
color_sky = (135, 206, 235) # Couleur du ciel

# Matériaux
mat_sphere_diffuse = {'type': 'diffuse', 'color': (255, 0, 0)} # Rouge mat
mat_plane_diffuse = {'type': 'diffuse', 'color': (150, 150, 150)} # Sol gris mat
mat_sphere_metal = {'type': 'metal', 'color': (200, 200, 200), 'roughness': 0.0} # Miroir parfait
mat_sphere_metal_b = {'type': 'metal', 'color': (200, 150, 50), 'roughness': 0.3} # Or brossé
mat_sphere_glass = {'type': 'dielectric', 'color': (255, 255, 255), 'ior': 1.5} # Verre

# Liste des objets
scene_objects = [
    # La grosse sphère centrale est en Verre
    Sphere(Vector(0, 0, -3), 1.0, mat_sphere_glass),
    
    # Le sol
    Plane(Vector(0, -1, 0), Vector(0, 1, 0), mat_plane_diffuse),
    
    # Une petite sphère miroir à gauche
    Sphere(Vector(-2.5, 0, -3), 0.5, mat_sphere_metal),
    
    # Une petite sphère en or brossé à droite
    Sphere(Vector(2.5, 0, -3), 0.5, mat_sphere_metal_b)
]

# --- Rendu ---
# Ouvre un fichier .ppm (que tu peux ouvrir avec GIMP ou un visionneur en ligne)
with open("scene_final_glass.ppm", "w") as f:
    f.write("P3\n")
    f.write(f"{image_width} {image_height}\n")
    f.write("255\n")

    # Boucle principale (de haut en bas)
    for y in range(image_height - 1, -1, -1):
        print(f"Lignes restantes: {y}", end='\r') # Barre de progression
        
        # Boucle de gauche à droite
        for x in range(image_width):
            
            # Initialise la couleur pour l'anti-aliasing
            total_color_r = 0
            total_color_g = 0
            total_color_b = 0

            # Boucle de sur-échantillonnage (Anti-aliasing)
            for s in range(samples_per_pixel):
                
                # Coordonnées aléatoires à l'intérieur du pixel
                u = (x + random.random() - image_width / 2) / (image_width / 2)
                v = (y + random.random() - image_height / 2) / (image_height / 2)
                aspect_ratio = image_width / image_height
                u *= aspect_ratio
                
                # Lancer le rayon
                ray_direction = Vector(u, v, -1).normalize()
                
                # Obtenir la couleur pour ce rayon
                color_sample = ray_color(camera_origin, ray_direction, MAX_DEPTH)
                
                # Ajouter à la couleur totale
                total_color_r += color_sample[0]
                total_color_g += color_sample[1]
                total_color_b += color_sample[2]
            
            # Faire la moyenne des couleurs du pixel
            avg_r = total_color_r / samples_per_pixel
            avg_g = total_color_g / samples_per_pixel
            avg_b = total_color_b / samples_per_pixel
            
            # Écrire le pixel dans le fichier
            f.write(f"{int(avg_r)} {int(avg_g)} {int(avg_b)}\n")

print("\nImage 'scene_final_glass.ppm' générée !")
