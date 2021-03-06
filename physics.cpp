#include <Box2D/Box2D.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <unordered_map>

typedef struct WinCapsule {
    b2Body *body;
    Window window;
} WinCapsule;

b2Vec2 grav(0, 5000);
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
            groundBodyDef.position.Set(0.0f, sh / 10);
            leftBodyDef.position.Set(sh / 10, 0.0f);
            rightBodyDef.position.Set(sh / 10, 0.0f);

            b2Body *groundBody = worlds[i].CreateBody(&groundBodyDef);
                //    *leftBody = worlds[i].CreateBody(&leftBodyDef),
                //    *rightBody = worlds[i].CreateBody(&rightBodyDef);

            b2PolygonShape groundBox, leftBox, rightBox;
            groundBox.SetAsBox(10e6, 0);
            // leftBox.SetAsBox(0, sw / 20);
            // rightBox.SetAsBox(0, sw / 20);

            groundBody->CreateFixture(&groundBox, 0.0f);
            // leftBody->CreateFixture(&leftBox, 0.0f);
            // rightBody->CreateFixture(&rightBox, 0.0f);
        }
    }

// TODO have Y coordinates match on screen, just have gravity upside down
    void physicsUpdate(int dt, Display *d) {
        for (int w = 0; w < 10; w++) {
            worlds[w].Step(0.0016, 8, 3);
        }
	
        for (std::pair<int, WinCapsule> element : windowBodies)
        {
            XWindowAttributes attrs;
            XGetWindowAttributes(d, element.second.window, &attrs);
            b2Body *e = element.second.body;

            double x = e->GetPosition().x * 10 - (double) attrs.width / 2;
            double y = e->GetPosition().y * 10 - (double) attrs.height / 2;

            XMoveWindow(d, element.second.window, x, y);
        }
    }

    int addWindow(Display *d, Window w, int world) {
        XWindowAttributes attr;
        XGetWindowAttributes(d, w, &attr);

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(attr.x / 10 + attr.width / 20, attr.y / 10 + attr.height / 20);
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

        return lastFree++;
    }

    void removeWindow(int id) {
        b2Body *b = windowBodies[id].body;
        b->GetWorld()->DestroyBody(b);
        windowBodies.erase(id);
    }

    void recreateWindow(int id, float x, float y, float w, float h) {
        b2Fixture *f = windowBodies[id].body->GetFixtureList();
        b2FixtureDef fixtureDef;
        fixtureDef.friction = f->GetFriction();
        fixtureDef.restitution = f->GetRestitution();
        b2PolygonShape shape;
        shape.SetAsBox(w / 20, h / 20);
        fixtureDef.shape = &shape;
        fixtureDef.density = f->GetDensity();
        
        b2BodyDef b;
        b.type = b2_dynamicBody;
        b.position.Set(x / 10 + w / 20, y / 10 + h / 20);
        b.fixedRotation = true;

        b2World *world = windowBodies[id].body->GetWorld();
        b2Body *body = world->CreateBody(&b);
        body->CreateFixture(&fixtureDef);

        world->DestroyBody(windowBodies[id].body);
        windowBodies[id].body = body;
    }
}