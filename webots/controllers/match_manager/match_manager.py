#!/usr/bin/env python3
from controller import Supervisor
import sys
import math
import random

TIME_STEP = 64
SUPERVISOR_DEF = "MATCH_MANAGER"
BUOYS_DEF = "BUOYS_SOLID"

supervisor = Supervisor()
robot_node = supervisor.getFromDef(SUPERVISOR_DEF)

if robot_node is None:
    sys.stderr.write(f"No DEF {SUPERVISOR_DEF} node found in the current world file\n")
    sys.exit(1)

buoy_nodes_field = supervisor.getFromDef(BUOYS_DEF).getField("children")
BUOYS_POS = {
    "red": [
        (100, 670, 72.5, True),
        (400, 300, 72.5, True),
        (400, 2050, 72.5, True),
        (510, 2550, 72.5, True),
        (800, 1100, 72.5, True),
        (1080, 450, 72.5, True),
        (1200, 1730, 72.5, True),
        (1200, 2700, 72.5, True),
        (1650, 1335, 72.5, True),
        (1650, 1935, 72.5, True),
        (1955, 1005, 72.5, True),
        (1955, 1605, 72.5, True),
        (1450, -67, 79.5, False),
        (1600, -67, 79.5, False),
        (1750, -67, 79.5, False),
        (1525, 3067, 79.5, False),
        (1675, 3067, 79.5, False),
    ],
    "green": [
        (100, 2330, 72.5, True),
        (400, 950, 72.5, True),
        (400, 2700, 72.5, True),
        (510, 450, 72.5, True),
        (800, 1900, 72.5, True),
        (1080, 2550, 72.5, True),
        (1200, 300, 72.5, True),
        (1200, 1270, 72.5, True),
        (1650, 1065, 72.5, True),
        (1650, 1665, 72.5, True),
        (1955, 1395, 72.5, True),
        (1955, 1995, 72.5, True),
        (1525, -67, 79.5, False),
        (1675, -67, 79.5, False),
        (1450, 3067, 79.5, False),
        (1600, 3067, 79.5, False),
        (1750, 3067, 79.5, False),
    ],
    "random": {
        1: {
            "red": [(-67, 775, 79.5, False), (-67, 1000, 79.5, False), (-67, 2075, 79.5, False), (-67, 2150, 79.5, False), (-67, 2300, 79.5, False)],
            "green": [(-67, 700, 79.5, False), (-67, 850, 79.5, False), (-67, 925, 79.5, False), (-67, 2000, 79.5, False), (-67, 2225, 79.5, False)],
        },
        2: {
            "red": [(-67, 850, 79.5, False), (-67, 1000, 79.5, False), (-67, 2075, 79.5, False), (-67, 2225, 79.5, False), (-67, 2300, 79.5, False)],
            "green": [(-67, 700, 79.5, False), (-67, 775, 79.5, False), (-67, 925, 79.5, False), (-67, 2000, 79.5, False), (-67, 2150, 79.5, False)],
        },
        3: {
            "red": [(-67, 925, 79.5, False), (-67, 1000, 79.5, False), (-67, 2150, 79.5, False), (-67, 2225, 79.5, False), (-67, 2300, 79.5, False)],
            "green": [(-67, 700, 79.5, False), (-67, 775, 79.5, False), (-67, 850, 79.5, False), (-67, 2000, 79.5, False), (-67, 2075, 79.5, False)],
        },
    },
}


def map_to_webots(pos):
    return [ axis/1000 for axis in pos ]

def spawn_buoy(pos_, color):
    *pos, reverse = pos_
    pos = map_to_webots(pos_)
    orientation_str = "1 0 0 1.5708" if not reverse else "0 -0.707107 0.707107 3.14159"

    buoy_node_str = (
        f"Buoy {{\n"
        f"  translation {pos[0]} {pos[1]} {pos[2]}\n"
        f"  rotation {orientation_str}\n"
        f'  color "{color}"\n'
        f"}}"
    )
    buoy_nodes_field.importMFNodeFromString(0, buoy_node_str)


def clear_buoys():
    for _ in range(buoy_nodes_field.getCount()):
        buoy_nodes_field.removeMF(0)


def generate_table():
    # Remove all buoys
    clear_buoys()

    # Get random configuration
    random_buoys_pos = BUOYS_POS["random"][random.randint(1, 3)]

    # Spawn buoys at predefined positions
    for pos in BUOYS_POS["red"]:
        spawn_buoy(pos, "red")

    for pos in random_buoys_pos["red"]:
        spawn_buoy(pos, "red")

    for pos in BUOYS_POS["green"]:
        spawn_buoy(pos, "green")

    for pos in random_buoys_pos["green"]:
        spawn_buoy(pos, "green")

    # Spawn buoys at random positions
    CIRCLE_RADIUS = 500
    BUOY_RADIUS = 36

    dist = lambda p, q: math.sqrt(sum((px - qx) ** 2.0 for px, qx in zip(p, q)))

    random_positions = []
    random_colors = ["red", "red", "red", "green", "green", "green"]
    random.shuffle(random_colors)

    for i, color in enumerate(random_colors):
        if i < 3:
            minAngle = -math.pi / 2
            maxAngle = 0
        else:
            minAngle = 0
            maxAngle = math.pi / 2

        while True:
            rho, theta = (random.uniform(0, CIRCLE_RADIUS - BUOY_RADIUS), random.uniform(minAngle, maxAngle))
            x = rho * math.cos(theta)
            y = rho * math.sin(theta)

            if x < BUOY_RADIUS or abs(y) < BUOY_RADIUS:
                continue

            candidate_pos = (x, y)

            for pos in random_positions:
                if dist(candidate_pos, pos) <= 2 * BUOY_RADIUS:
                    break
            else:
                random_positions.append(candidate_pos)
                spawn_buoy([candidate_pos[0], 1500 + candidate_pos[1], 72.5], color)
                break


generate_table()

while supervisor.step(TIME_STEP) != -1:
    pass
