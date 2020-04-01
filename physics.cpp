#include <Box2D/Box2D.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <unordered_map>

typedef struct WinCapsule {
    b2Body *body;
    Window window;
} WinCapsule;

b2Vec2 grav(0.0f, -1000);
double sh, sw;

std::unordered_map<int, WinCapsule> windowBodies;
int lastFree = 0;

// 10 workspaces, 1 world per workspace
// Really ugly but idk c++
b2World worlds[10] = {
    b2World(grav),
    b2World(grav),
    b2World(grav),
    b2World(grav),
    b2World(grav),
    b2World(grav),
    b2World(grav),
    b2World(grav),
    b2World(grav),
    b2World(grav),
};

extern "C" {
    void physicsInit(int w, int h) {
        sw = (double) w;
        sh = (double) h;
        // initialize all workspaces
        for (int i = 0; i < 10; i++) {
            // Needless to say this code is terrible
            // 10px = 1m
            b2BodyDef groundBodyDef;
            b2BodyDef leftBodyDef;
            b2BodyDef rightBodyDef;
            groundBodyDef.position.Set(0.0f, -sh / 10);
            leftBodyDef.position.Set(-sh / 10, 0.0f);
            rightBodyDef.position.Set(sh / 10, 0.0f);

            b2Body *groundBody = worlds[i].CreateBody(&groundBodyDef),
                   *leftBody = worlds[i].CreateBody(&leftBodyDef),
                   *rightBody = worlds[i].CreateBody(&rightBodyDef);

            b2PolygonShape groundBox, leftBox, rightBox;
            groundBox.SetAsBox(sh / 20, 5);
            leftBox.SetAsBox(0.05f, sw / 20);
            rightBox.SetAsBox(0.05f, sw / 20);

            groundBody->CreateFixture(&groundBox, 0.0f);
            leftBody->CreateFixture(&leftBox, 0.0f);
            rightBody->CreateFixture(&rightBox, 0.0f);

            printf("Created world for workspace #%d\n", i);
        }
    }

    void physicsUpdate(int dt, Display *d) {
        printf("Called physics update %d\n", dt);
        for (int w = 0; w < 10; w++) {
            worlds[w].Step(0.0016, 8, 3);
        }
	
        for (std::pair<int, WinCapsule> element : windowBodies)
        {
            XWindowAttributes attrs;
            XGetWindowAttributes(d, element.second.window, &attrs);
            b2Body *e = element.second.body;

            int x = e->GetPosition().x * 10;
            int y = -e->GetPosition().y * 10;


            printf("Moving window to %d %d\n", x, y);
            printf("Window was at %d %d\n", attrs.x, attrs.y);
            XMoveWindow(d, element.second.window, x, y);
        }
        printf("---\n");
    }

    int addWindow(Display *d, Window w, int world) {
        XWindowAttributes attr;
        XGetWindowAttributes(d, w, &attr);

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(attr.x / 10, -attr.y / 10);
        bodyDef.fixedRotation = true;
        b2Body *windowBody = worlds[world].CreateBody(&bodyDef);
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(attr.width / 20, attr.height / 20);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.restitution = 0.5;

        windowBody->CreateFixture(&fixtureDef);

        windowBodies[lastFree] = (WinCapsule) { .body = windowBody, .window = w };

        printf("Added a window!\n");

        return lastFree++;
    }

    void removeWindow(int id) {
        b2Body *b = windowBodies[id].body;
        b->GetWorld()->DestroyBody(b);
        windowBodies.erase(id);
    }
}