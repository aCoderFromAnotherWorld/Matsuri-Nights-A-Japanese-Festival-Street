#!/usr/bin/env python3
"""
Generate vector PDF & high-res PNG diagrams for the Matsuri Nights Final Report.
Course: CSE4102 - Computer Graphics and Image Processing Laboratory, KUET
Author: MD. Abu Hasanat Soykot (2107100)
"""

import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.path import Path

# Configure publication-quality typography
plt.rcParams.update({
    'font.family': 'serif',
    'font.size': 10,
    'axes.labelsize': 11,
    'axes.titlesize': 12,
    'legend.fontsize': 9,
    'xtick.labelsize': 9,
    'ytick.labelsize': 9,
    'figure.autolayout': True,
    'figure.dpi': 300,
    'pdf.fonttype': 42,
    'ps.fonttype': 42
})

OUTPUT_DIR = os.path.dirname(os.path.abspath(__file__))

def save_fig(fig, base_name):
    pdf_path = os.path.join(OUTPUT_DIR, f"{base_name}.pdf")
    png_path = os.path.join(OUTPUT_DIR, f"{base_name}.png")
    fig.savefig(pdf_path, bbox_inches='tight')
    fig.savefig(png_path, bbox_inches='tight', dpi=300)
    plt.close(fig)
    print(f"Generated: {base_name}.pdf and {base_name}.png")

# =========================================================================
# 1. Render Pipeline & Multi-Pass Frame Flow Diagram
# =========================================================================
def make_render_pipeline():
    fig, ax = plt.subplots(figsize=(9.5, 4.2))
    ax.axis('off')

    stages = [
        ("1. Input & Timing", "glfwPollEvents()\ndeltaTime, FPS Window\nContinuous Movement\nCollision & Portals", "#EBF5FB", "#2980B9"),
        ("2. Scene & Physics", "Scene::update()\nKinematic Cycles\nDay/Night State Machine\nUpdate 16 Lights", "#E8F8F5", "#16A085"),
        ("3. Shadow Depth Pass", "2048x2048 FBO\nglCullFace(GL_FRONT)\nshadow_depth.vert/.frag\nOrtho Light Matrix", "#FEF9E7", "#D4AC0D"),
        ("4. Main 3D Shading", "Default FBO (1280x720)\n16 Active Dynamic Lights\n16-Sample PCF Filter\nbasic.vert / basic.frag", "#FDEDEC", "#C0392B"),
        ("5. HUD & UI Overlay", "Orthographic 2D Pass\nAlpha Blending\nConsolas Font Atlas\nMinimal Telemetry", "#F4ECF7", "#8E44AD"),
        ("6. Export & Swap", "Screenshot Capture\nglReadPixels() to PNG/BMP\nglfwSwapBuffers()\nFrame Completion", "#EAECEE", "#34495E")
    ]

    box_width = 1.35
    box_height = 2.4
    spacing = 0.22
    start_x = 0.2
    y = 0.9

    for i, (title, desc, bg_col, border_col) in enumerate(stages):
        x = start_x + i * (box_width + spacing)
        rect = patches.FancyBboxPatch(
            (x, y), box_width, box_height,
            boxstyle="round,pad=0.08,rounding_size=0.12",
            facecolor=bg_col, edgecolor=border_col, linewidth=1.8
        )
        ax.add_patch(rect)
        ax.text(x + box_width/2, y + box_height - 0.35, title,
                ha='center', va='center', weight='bold', color=border_col, fontsize=10)
        ax.plot([x + 0.1, x + box_width - 0.1], [y + box_height - 0.65, y + box_height - 0.65],
                color=border_col, linewidth=0.8, alpha=0.6)
        ax.text(x + box_width/2, y + (box_height - 0.75)/2, desc,
                ha='center', va='center', fontsize=8.2, linespacing=1.35, color='#2C3E50')

        if i < len(stages) - 1:
            arr_x = x + box_width + 0.02
            arr_dx = spacing - 0.04
            ax.annotate('', xy=(arr_x + arr_dx, y + box_height/2), xytext=(arr_x, y + box_height/2),
                        arrowprops=dict(arrowstyle="-|>", color='#5D6D7E', lw=2.0, mutation_scale=12))

    ax.set_xlim(0, start_x + len(stages) * (box_width + spacing))
    ax.set_ylim(0.4, 3.8)
    save_fig(fig, "fig_render_pipeline")

# =========================================================================
# 2. Hierarchical Scene Graph Tree Diagram
# =========================================================================
def make_scene_graph():
    fig, ax = plt.subplots(figsize=(10.0, 5.0))
    ax.axis('off')

    nodes = {
        'root': ('World_Root\n(Origin [0,0,0])', 4.5, 4.2, '#2C3E50', 'white'),
        'ground': ('GroundObject\nPavement & Verges', 0.8, 2.7, '#27AE60', 'white'),
        'machiya': ('Machiya Buildings\n(4 Two-Story Houses)', 2.3, 2.7, '#D35400', 'white'),
        'torii': ('Torii Gate\nShrine Terminus', 3.8, 2.7, '#C0392B', 'white'),
        'spans': ('Street Lanterns\n(4 Catenary Spans)', 5.3, 2.7, '#F39C12', 'white'),
        'stalls': ('Festival Stalls\nTakoyaki & Kakigori', 6.8, 2.7, '#8E44AD', 'white'),
        'stage': ('Magic Stage\nPerformance Rig', 8.3, 2.7, '#2980B9', 'white'),

        # Children of Machiya
        'door': ('Sliding Doors\n(Interactive H)', 1.7, 1.1, '#EDBB99', '#6E2C00'),
        'window': ('Shoji Windows\n(Interactive G)', 2.9, 1.1, '#EDBB99', '#6E2C00'),

        # Children of Torii
        'chochin': ('Chochin Lanterns\n(4 Emissive Glow)', 3.8, 1.1, '#FADBD8', '#78281F'),

        # Children of Lantern Spans
        'rope': ('Rope Pivot\n(Pendulum Swing)', 4.8, 1.1, '#FCF3CF', '#7D6608'),
        'lamp': ('Lantern Body\n(Point Lights 3, 4)', 5.8, 1.1, '#FCF3CF', '#7D6608'),

        # Children of Stage
        'magician': ('Magician & Rig\n(Articulated Figure)', 7.8, 1.1, '#D4E6F1', '#1B4F72'),
        'orb': ('Magic Orb\n(Point Light 0)', 8.9, 1.1, '#D4E6F1', '#1B4F72'),
    }

    edges = [
        ('root', 'ground'), ('root', 'machiya'), ('root', 'torii'),
        ('root', 'spans'), ('root', 'stalls'), ('root', 'stage'),
        ('machiya', 'door'), ('machiya', 'window'),
        ('torii', 'chochin'),
        ('spans', 'rope'), ('rope', 'lamp'),
        ('stage', 'magician'), ('magician', 'orb')
    ]

    for parent, child in edges:
        px, py = nodes[parent][1], nodes[parent][2]
        cx, cy = nodes[child][1], nodes[child][2]
        ax.plot([px, cx], [py - 0.25, cy + 0.35], color='#7F8C8D', lw=1.5, zorder=1)

    for key, (label, x, y, bg_col, text_col) in nodes.items():
        w, h = 1.15, 0.70
        rect = patches.FancyBboxPatch(
            (x - w/2, y - h/2), w, h,
            boxstyle="round,pad=0.05,rounding_size=0.1",
            facecolor=bg_col, edgecolor='#34495E', linewidth=1.2, zorder=2
        )
        ax.add_patch(rect)
        ax.text(x, y, label, ha='center', va='center', fontsize=7.8,
                weight='bold', color=text_col, zorder=3, linespacing=1.2)

    ax.set_xlim(0, 9.8)
    ax.set_ylim(0.4, 4.8)
    save_fig(fig, "fig_scene_graph_tree")

# =========================================================================
# 3. Blinn-Phong Illumination & Halfway Vector Geometry
# =========================================================================
def make_blinn_phong_vectors():
    fig, ax = plt.subplots(figsize=(6.2, 4.4))

    # Surface line
    ax.plot([-2.0, 2.0], [0, 0], color='#2C3E50', lw=3.0, label='Surface Tangent')
    ax.fill_between([-2.0, 2.0], -0.4, 0, color='#EAEDED', hatch='///')

    origin = np.array([0.0, 0.0])

    # Normal vector N
    N = np.array([0.0, 1.8])
    # Light vector L
    L = np.array([-1.2, 1.5])
    # View vector V
    V = np.array([1.4, 1.4])
    # Reflection vector R
    R = np.array([1.2, 1.5])
    # Halfway vector H
    H = (L/np.linalg.norm(L) + V/np.linalg.norm(V))
    H = (H / np.linalg.norm(H)) * 1.8

    vectors = [
        (N, r'$\mathbf{N}$ (Normal)', '#27AE60', 1.8),
        (L, r'$\mathbf{L}$ (Light)', '#F39C12', 1.8),
        (V, r'$\mathbf{V}$ (View)', '#2980B9', 1.8),
        (R, r'$\mathbf{R}$ (Reflection)', '#BDC3C7', 1.4),
        (H, r'$\mathbf{H} = \frac{\mathbf{L}+\mathbf{V}}{\|\mathbf{L}+\mathbf{V}\|}$', '#C0392B', 2.2)
    ]

    for vec, label, col, lw in vectors:
        ax.annotate('', xy=(vec[0], vec[1]), xytext=(0, 0),
                    arrowprops=dict(arrowstyle="-|>", color=col, lw=lw, mutation_scale=15))
        offset_x = 0.08 if vec[0] >= 0 else -0.15
        offset_y = 0.08
        ax.text(vec[0] + offset_x, vec[1] + offset_y, label, color=col,
                weight='bold', fontsize=9.5, ha='center')

    # Draw angle arcs
    arc_rad = 0.6
    theta = np.linspace(np.pi/2, np.arctan2(L[1], L[0]), 30)
    ax.plot(arc_rad * np.cos(theta), arc_rad * np.sin(theta), color='#F39C12', lw=1.2, ls='--')
    ax.text(-0.25, 0.75, r'$\theta$', color='#F39C12', fontsize=10, weight='bold')

    arc_rad_h = 0.9
    psi = np.linspace(np.pi/2, np.arctan2(H[1], H[0]), 30)
    ax.plot(arc_rad_h * np.cos(psi), arc_rad_h * np.sin(psi), color='#C0392B', lw=1.2, ls=':')
    ax.text(0.12, 1.05, r'$\psi$', color='#C0392B', fontsize=10, weight='bold')

    ax.set_xlim(-2.2, 2.2)
    ax.set_ylim(-0.5, 2.3)
    ax.set_aspect('equal')
    ax.axis('off')
    save_fig(fig, "fig_blinn_phong_vectors")

# =========================================================================
# 4. Day / Night State Machine & Illumination Modulation Curves
# =========================================================================
def make_day_night_curves():
    fig, ax = plt.subplots(figsize=(6.5, 3.8))

    factor = np.linspace(0.0, 1.0, 200)

    # Directional sun intensity (drops from 1.0 to 0.20)
    sun_intensity = 0.85 * (1.0 - factor) + 0.20 * factor
    # Ambient indoor bounce
    ambient_bounce = 0.45 * (1.0 - factor) + 0.16 * factor
    # Lantern & Stall point lights boost (ramps up from 0.20 to 1.20)
    lantern_boost = 0.20 * (1.0 - factor) + 1.20 * factor
    # Twinkling star intensity in sky (non-linear pow)
    star_intensity = np.power(factor, 1.25) * 1.5

    ax.plot(factor, sun_intensity, label='Sun Directional Diffuse', color='#F39C12', lw=2.2)
    ax.plot(factor, ambient_boost := lantern_boost, label='Point Lights (Lanterns & Stalls)', color='#C0392B', lw=2.2)
    ax.plot(factor, ambient_bounce, label='Ambient Light (Base Floor)', color='#2980B9', lw=1.8, ls='--')
    ax.plot(factor, star_intensity, label='Nocturnal Star Twinkle', color='#8E44AD', lw=1.8, ls=':')

    ax.set_xlabel('Day/Night Factor ($0.0 = \\text{Day}, 1.0 = \\text{Festival Night}$)')
    ax.set_ylabel('Normalized Illumination Factor')
    ax.set_title('Celestial & Artificial Light Modulation Across Day/Night Cycle')
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(loc='center left', frameon=True)
    ax.set_xlim(0, 1.0)
    ax.set_ylim(0, 1.6)

    save_fig(fig, "fig_day_night_curves")

# =========================================================================
# 5. Shadow Mapping & 16-Sample PCF Kernel Filtering
# =========================================================================
def make_shadow_pcf():
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(8.5, 3.8))

    # Left: Shadow Depth Pass Geometry
    ax1.plot([-1.5, 1.5], [-1.0, -1.0], color='#2C3E50', lw=2.5)
    # Occluder box
    box = patches.Rectangle((-0.5, -0.2), 1.0, 0.6, facecolor='#E74C3C', edgecolor='#922B21', lw=1.5)
    ax1.add_patch(box)
    ax1.text(0.0, 0.1, 'Occluder\n(Machiya Eave)', ha='center', va='center', color='white', weight='bold', fontsize=8)

    # Light source
    ax1.scatter([0.0], [1.5], color='#F39C12', s=120, zorder=5)
    ax1.text(0.0, 1.7, 'Light Source (Sun)', ha='center', color='#B9770E', weight='bold', fontsize=9)

    # Light rays
    ax1.plot([0, -1.2], [1.5, -1.0], color='#F39C12', ls='--', lw=1.2)
    ax1.plot([0, 1.2], [1.5, -1.0], color='#F39C12', ls='--', lw=1.2)
    ax1.plot([0, -0.5], [1.5, 0.4], color='#F39C12', lw=1.2)
    ax1.plot([0, 0.5], [1.5, 0.4], color='#F39C12', lw=1.2)

    # Shadow zone on ground
    ax1.plot([-0.7, 0.7], [-1.0, -1.0], color='#2C3E50', lw=5.0)
    ax1.text(0.0, -1.3, 'Penumbra Shadow Zone', ha='center', color='#2C3E50', weight='bold', fontsize=8.5)

    ax1.set_xlim(-1.8, 1.8)
    ax1.set_ylim(-1.6, 2.0)
    ax1.set_aspect('equal')
    ax1.axis('off')
    ax1.set_title('(a) Light-Space Depth Occlusion', fontsize=10.5)

    # Right: 16-Sample PCF Kernel Grid
    kernel_size = 4
    for x in range(kernel_size):
        for y in range(kernel_size):
            cx, cy = x - 1.5, y - 1.5
            dist = np.sqrt(cx**2 + cy**2)
            alpha = max(0.2, 1.0 - dist / 2.5)
            circle = plt.Circle((cx, cy), 0.28, color='#2980B9', alpha=alpha)
            ax2.add_patch(circle)
            ax2.text(cx, cy, f"S{y*4+x+1}", ha='center', va='center', fontsize=6.8, color='white', weight='bold')

    ax2.set_xlim(-2.5, 2.5)
    ax2.set_ylim(-2.5, 2.5)
    ax2.set_aspect('equal')
    ax2.grid(True, linestyle=':', alpha=0.6)
    ax2.set_xlabel(r'Texel Offset $u$ ($\Delta = 1.2 / 2048$)')
    ax2.set_ylabel(r'Texel Offset $v$ ($\Delta = 1.2 / 2048$)')
    ax2.set_title(r'(b) 16-Sample PCF Filter Kernel Disc', fontsize=10.5)

    save_fig(fig, "fig_shadow_pcf")

# =========================================================================
# 6. Catenary Sagging Curve vs Parametric Bézier Formulations
# =========================================================================
def make_catenary_bezier():
    fig, ax = plt.subplots(figsize=(6.5, 3.8))

    L = 3.8
    y_pole = 6.2
    sag = 0.65

    x = np.linspace(-L, L, 200)
    normX = x / L

    # Parabolic catenary approximation from Curves.h: createCatenaryRope
    y_code = y_pole - sag * (1.0 - normX**2)

    # True hyperbolic catenary: y = a * cosh(x/a) + C
    a = (L**2) / (2.0 * sag)
    y_true_catenary = y_pole - sag + a * (np.cosh(x / a) - 1.0)

    # Cubic Bezier matching endpoints and middle dip
    P0 = np.array([-L, y_pole])
    P1 = np.array([-L * 0.5, y_pole - sag * 1.33])
    P2 = np.array([L * 0.5, y_pole - sag * 1.33])
    P3 = np.array([L, y_pole])

    t = np.linspace(0, 1, 200)
    u = 1.0 - t
    bezier_x = (u**3)*P0[0] + 3*(u**2)*t*P1[0] + 3*u*(t**2)*P2[0] + (t**3)*P3[0]
    bezier_y = (u**3)*P0[1] + 3*(u**2)*t*P1[1] + 3*u*(t**2)*P2[1] + (t**3)*P3[1]

    ax.plot(x, y_code, label=r'Code Catenary: $y = y_{\mathrm{pole}} - s(1 - (x/L)^2)$',
            color='#C0392B', lw=2.2)
    ax.plot(x, y_true_catenary, label=r'Ideal Catenary: $y = a\cosh(x/a) - a$',
            color='#2980B9', lw=1.8, ls='--')
    ax.plot(bezier_x, bezier_y, label='Cubic Bézier Sweep ($\mathbf{B}_3(t)$)',
            color='#27AE60', lw=1.8, ls=':')

    # Mark support poles
    ax.scatter([-L, L], [y_pole, y_pole], color='#2C3E50', s=70, zorder=5)
    ax.text(-L, y_pole + 0.1, 'Pole Left', ha='center', weight='bold', fontsize=8.5)
    ax.text(L, y_pole + 0.1, 'Pole Right', ha='center', weight='bold', fontsize=8.5)

    ax.set_xlabel('Horizontal Span $x$ (Meters)')
    ax.set_ylabel('Vertical Elevation $y$ (Meters)')
    ax.set_title('Parametric Sagging Curve Profiles for Sacred Shimenawa Ropes')
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(loc='lower center', frameon=True)
    ax.set_ylim(5.2, 6.7)

    save_fig(fig, "fig_catenary_bezier")

if __name__ == '__main__':
    print("Generating academic report diagrams...")
    make_render_pipeline()
    make_scene_graph()
    make_blinn_phong_vectors()
    make_day_night_curves()
    make_shadow_pcf()
    make_catenary_bezier()
    print("All diagrams generated successfully in report/figures/!")
