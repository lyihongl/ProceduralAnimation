from typing import Tuple, Dict, List, Set
import math
import util

# LineSeg = Tuple[float, float, Tuple[float, float]]
Point = Tuple[float, float]

# class Line:
#     def __init__(self, length:float):
#         self.length = length


class Thing:
    def __init__(self, start: Tuple[float, float]):
        # self.line_to_point = {}
        self.connections: Dict[int, List[Tuple[int, float]]] = {}
        self.start = start

        # self.line_segments = []
        self.points: List[Tuple[float, float]] = []
        self.lock: Set[int] = set()

    def addPoint(self):
        self.points.append(self.start)
        return len(self.points)-1

    def addConnection(self, a: int, b: int, d: float):
        if(a == b):
            raise "Invalid inputs {} {}".format(a, b)
        if(a not in self.connections):
            self.connections[a] = []
        if(b not in self.connections):
            self.connections[b] = []
        self.connections[a].append((b, d))
        self.connections[b].append((a, d))

    def initPlacement(self):
        placed = set()
        for i in range(0, len(self.points)):

            if(i not in self.connections):
                continue
            if(i in placed):
                continue
            if(i == 0):
                self.points[i] = self.start
                placed.add(i)
                continue

            for b, d in self.connections[i]:
                # print("i", i)
                if(abs(math.dist(self.points[i], self.points[b]) - d) <= 0.001):
                    continue

                if(b in placed):
                    placed.add(i)
                    print(i, self.points[b])
                    self.points[i] = (self.points[b][0], self.points[b][1]+d)

    def update(self):
        for i in range(0, len(self.points)):
            if(i in self.lock):
                continue

            point = self.points[i]
            self.lock.add(i)

            if(len(self.connections[i]) == 0):
                continue

            other = 0
            top_dist = (0, other)
            # print("dist", i, top_dist)
            # print("connections", self.connections)
            for ii, p in enumerate(self.connections[i]):
                # print("i, ii", i, ii)
                # p = self.connections[i][ii][0]
                # print("i, p", i, p, self.points[p], point)
                dist = math.dist(self.points[p[0]], point)
                if(dist > top_dist[0]):
                    # print("c[0]", ii)
                    top_dist = (dist, ii)

            other = top_dist[1]
            # print("i: {}, top dist {}, p[i] {}, p[other] {}".format(i, top_dist, point, self.points[self.connections[i][other][0]]))
            if(top_dist[0] < self.connections[i][other][1]):
                # print("i", i)
                continue
            # print("i, top_dist", i, top_dist)

            direction = util.add_vec(
                self.points[self.connections[i][other][0]], util.scalar(-1, point))
            # print(self.connections[i])
            # print("i, direction", i, direction)

            angle = math.atan2(direction[1], direction[0])

            adj_dist = self.connections[i][other][1]
            # print(angle, adj_dist)

            y = math.sin(angle)*adj_dist
            x = math.cos(angle)*adj_dist
            # print("(x,y):({}, {})".format(x, y))

            self.points[i] = util.add_vec(self.points[self.connections[i][other][0]], (-x, -y))

            # slope = ("normal", 0)

            # dy = 0
            # dx = 0
            # if direction[0] == 0:
            #     if direction[1] > 0:
            #         dy = direction[1] - self.connections[i][other][0]
            #     else:
            #         dy = direction[1] + self.connections[i][other][0]
            # elif direction[1] == 0:
            #     if direction[1] > 0:
            #         dx = direction[0] - self.connections[i][other][0]
            #     else:
            #         dx = direction[0] + self.connections[i][other][0]
            #     # slope = ("horizontal")
            # else:
            #     slope = direction[1]/direction[0]
            #     dy = slope*(top_dist[0] - 50)
            #     dx = 1/slope*(top_dist[0] - 50)
            # print(i, dx, dy)
            # self.points[i] = util.add_vec(self.points[i], (dx, dy))
            # dy = slope*(top_dist[0])
            # slope = ("normal", direction[1]/direction[0])

            # print(math.atan2(direction[1], direction[0]), direction)
            # if(direction[0] == 0):
            #     self.points[other] = (self.points[other][0], self.)
            #     pass
            # elif(direction[1] == 0):
            #     pass
            # else:
            #     pass
            # polar = (math.dist((0, 0), direction), math.atan2(direction[1], direction[0]))

            # print(polar)
            # print(direction)
            # print(top_dist)

    def move(self, p: Tuple[float, float]):
        # print(self.points)
        self.lock.add(0)
        self.points[0] = p
        self.update()
        self.lock.clear()
        # print(self.points)
    # def add_line_seg(self, l: LineSeg):
    #     self.line_segments.append(l)
    #     return len(self.line_segments)-1
