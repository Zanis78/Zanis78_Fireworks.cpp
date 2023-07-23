#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>

using namespace sf;

const int W = 1200;
const int H = 800;

Time dt;

float RandomFloat(float fMax)
{
    return ((float)rand() / (float)RAND_MAX) * fMax;
}

class Entity
{
public:
    int r, g, b,t;
    float x = 0;
    float y = 0;
    float dx = 0;
    float dy = 0;
    float fuse = 0;
    float lifetime = 0;
    float R = 1;
    float fGravity = 250.0f;
    float fGravityontime;
    int iPower = 0;
    bool life;
    std::string name;

    Entity()
    {
        life = 1;
    }

    void settings(int X, int Y, int radius = 1)
    {
        x = X; y = Y;
        R = radius;
    }

    virtual void update() {};

    void draw(RenderWindow& app)
    {
        CircleShape circle(R);
        circle.setFillColor(Color(r, g, b, t));
        circle.setPosition(x, y);
        circle.setOrigin(R, R);
        app.draw(circle);
    }

    virtual ~Entity() {};
};

class rocket : public Entity
{
public:
    rocket()
    {
        r = 255;    
        g = 200;    
        b = 200;    
        t = 255;    
        R = 2.0f;
        dx = (RandomFloat(100.0f) - 50.0f) * 5;
        dy = -500.f;
        fuse = RandomFloat(1.f) + 0.7f;
        iPower = (RandomFloat(500.0f) + 20.0f); //number of stars after explode
        name = "rocket";
    }

    void update()
    {
        fGravityontime = lifetime / fuse * fGravity;
        x += dx * dt.asSeconds();
        y += (dy + fGravityontime) * dt.asSeconds();
        lifetime += dt.asSeconds();
        if (lifetime >= fuse) life = false;        
    }

};

class stars : public Entity
{
public:

    float fDrag = 0.999;
    float angle;
    float powerex;
    
    stars()
    {
        angle = RandomFloat(2.0f * 3.14159f);
        powerex = RandomFloat(300.0f) + 50.f;
        dx = cos(angle) * powerex;
        dy = sin(angle) * powerex;
        fuse = RandomFloat(1.5f) + 0.5f;
        name = "stars";
        t = 255;        //transparency
    }

    void update()
    {
        fGravityontime = lifetime / fuse * fGravity;
        t = ((1 - (lifetime / fuse)) * 255);
        x += dx * fDrag * dt.asSeconds();
        y += (dy + fGravityontime) * dt.asSeconds();
        lifetime += dt.asSeconds();
        if (lifetime >= fuse) life = false;
        //(lifetime < fuse * 0.75) ? t = 255 : t = 255 / 2;
    }
};

int main()
{
    sf::Clock clock;
    std::list<Entity*> entities;
    float timeCount = 0.f;
    float rockeLaunchFreq;
    bool bHasExploded;

    srand(time(0));

    RenderWindow app(VideoMode(W, H), "Asteroids!"); 

    while (app.isOpen())
    {
        dt = clock.restart();
        timeCount += dt.asSeconds();
        rockeLaunchFreq = RandomFloat(5.0f) + 1.0f;
        bHasExploded = true;

        if (timeCount >= rockeLaunchFreq)
        {
            timeCount = 0;
            rocket* a = new rocket();
            a->settings(W / 2, H, 2);
            a->name = "rocket";
            entities.push_back(a);
        }

        sf::Event event;
        while (app.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                app.close();
        }

        for (auto i = entities.begin(); i != entities.end();)
        {
            Entity* e = *i;
            if (e->life == false && e->name == "rocket") 
            {
                if (bHasExploded)
                {
                    int colorR = int(RandomFloat(255.0f));                
                    int colorG = int(RandomFloat(255.0f));                
                    int colorB = int(RandomFloat(255.0f));
                
                        for (int i = 0; i < e->iPower; i++)
                        {
                              bHasExploded = false;
                              stars* a = new stars();
                              a->settings(e->x, e->y, 1);
                              a->x = e->x;
                              a->x = e->x;
                              a->name = "stars";
                              a->r = colorR;
                              a->g = colorG;
                              a->b = colorB;
                              entities.push_back(a);

                        }
                }
            }

            if (e->life == false)
            {
                i = entities.erase(i); 
                delete e;
            }
            else
                i++;
        }

        app.clear();
        for (auto i : entities)
        {
            i->update();
            i->draw(app);
        }
        app.display();
    }

    return 0;
}