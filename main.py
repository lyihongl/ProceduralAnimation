import pygame
import time
import thing
import util
import math

pygame.init()
WIDTH = 800
HEIGHT = 600
screen = pygame.display.set_mode([WIDTH, HEIGHT], pygame.SRCALPHA, 32)

running = True

prev_time = time.time()
spawn_timer_delta = time.time()
spawn_rate = 25
target_fps = 60
dt = 0

t = thing.Thing((100.0, 40.0))

a = [t.addPoint() for _ in range(0, 15)]
for i in range(1, len(a)):
    t.addConnection(a[i-1], a[i], 15.0)
# p1 = t.addPoint()
# p2 = t.addPoint()
# p3 = t.addPoint()
# p4 = t.addPoint()
# p5 = t.addPoint()

# t.addConnection(p1, p2, 50.0)
# t.addConnection(p2, p3, 50.0)
# t.addConnection(p3, p4, 50.0)
# t.addConnection(p4, p5, 50.0)
# t.addConnection(p5, p3, 50.0)
t.initPlacement()
print(t.connections)

# print(p1, p2)


def drawThing(s: pygame.Surface, t: thing.Thing):
    # for p in t.points:
    #     pygame.draw.circle(s, (255, 255, 255), p, 10, 1)

    drawnLines = set()
    for c in t.connections:
        for otherT in t.connections[c]:
            other = otherT[0]
            if(c in drawnLines and other in drawnLines):
                continue
        # other = t.connections[c][0]
            drawnLines.add(c)
            drawnLines.add(other)
            # pygame.draw.rect(s, (255, 255, 255), p)
            pygame.draw.line(s, (255, 255, 255),
                             t.points[c], t.points[other], 2)

            # direction = util.add_vec(
            #     t.points[c], util.scalar(-1, t.points[other]))
            
            # norm_dir = util.scalar(1/math.dist((0, 0), direction), (-direction[1], direction[0]))
            
            # p1 = util.add_vec(t.points[c], util.scalar(10, norm_dir))
            # p2 = util.add_vec(t.points[c], util.scalar(-10, norm_dir))
            # pygame.draw.line(s, (255, 255, 255), p1, p2, 2)
            # pygame.draw.circle(s, (255, 255, 255), p1, 2, 2)
            # pygame.draw.circle(s, (255, 255, 255), p2, 2, 2)
    
    for c in t.connections:
        if(len(t.connections[c]) != 2):
            continue
        p_c = t.points[c]
        p = [t.points[otherT[0]] for otherT in t.connections[c]]
        
        l1 = util.add_vec(p_c, util.scalar(-1, p[0]))
        l2 = util.add_vec(p_c, util.scalar(-1, p[1]))

        a1 = math.atan2(l1[1], l1[0])
        a2 = math.atan2(l2[1], l2[0])
        diffa = (a2 + a1)/2

        np1 = util.add_vec(p_c, util.scalar(13, (math.cos(diffa), math.sin(diffa))))
        np2 = util.add_vec(p_c, util.scalar(-13, (math.cos(diffa), math.sin(diffa))))
        pygame.draw.circle(s, (255, 255, 255), np1, 2, 2)
        pygame.draw.circle(s, (255, 255, 255), np2, 2, 2)



while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN:
            # t.move(pygame.mouse.get_pos())
            pass

    screen.fill((0, 0, 0))
    t.move(pygame.mouse.get_pos())
    drawThing(screen, t)
    pygame.display.flip()
    curr_time = time.time()  # so now we have time after processing
    diff = curr_time - prev_time  # frame took this much time to process and render
    # if we finished early, wait the remaining time to desired fps, else wait 0 ms!
    delay = max(1.0/target_fps - diff, 0)
    # init.gui_manager.update(delay+diff)
    time.sleep(delay)
    dt = delay+diff
    # fps is based on total time ("processing" diff time + "wasted" delay time)
    fps = 1.0/(dt)
    prev_time = curr_time
    pygame.display.set_caption(
        "{0}: {1:.2f} {2:.6f}".format("game test", fps, dt))
