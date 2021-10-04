struct Box {
    Box() {
        ids[0] = mainScene.addFigure(&v1);
        ids[1] = mainScene.addFigure(&v2);
        ids[2] = mainScene.addFigure(&h1);
        ids[3] = mainScene.addFigure(&h2);
        v1.setFilling(v);
        v2.setFilling(v);
        setState(0);
        h1.setFilling(h);
        h2.setFilling(h);
    }

    void setState(int _state) {
        state = _state;
        if (state == 2) {
            mainScene.enableFigure(ids[0]);
            mainScene.enableFigure(ids[1]);
            mainScene.enableFigure(ids[2]);
            mainScene.enableFigure(ids[3]);
        } else {
            mainScene.disableFigure(ids[0]);
            mainScene.disableFigure(ids[1]);
            mainScene.disableFigure(ids[2]);
            mainScene.disableFigure(ids[3]);
        }
    } 

    void setPos(COORD _position) {
        position = _position;
        v1.setPos({position.X, short(position.Y + 1)});
        v2.setPos({short (position.X + size.X), short (position.Y + 1)});
        h1.setPos({short (position.X + 1), position.Y});
        h2.setPos({short (position.X + 1), short (position.Y + size.Y)});
    }

    void setSize(COORD _size) {
        size = _size;
        short h = size.X - 1;
        short v = size.Y - 1;
        v1.setSize({1, v});
        v2.setSize({1, v});
        h1.setSize({h, 1});
        h2.setSize({h, 1});
    }

    void draw() {
        if (state == 0) return;
        screen.setPX_s(position, lt);
        screen.setPX_s({position.X, short(position.Y + size.Y)}, lb);
        screen.setPX_s({short (position.X + size.X), position.Y}, rt);
        screen.setPX_s({short (position.X + size.X), short (position.Y + size.Y)}, rb);
    }

    int state = 0;
    int ids[4];
    wchar_t v = L'│', h = L'─';
    wchar_t lt = L'┌', lb = L'└', rt = L'┐', rb = L'┘';
    COORD position = {0, 0}, size = {3, 3};
    shapes::Rectangle v1, v2, h1, h2;
} box;

struct Sphere {
    Sphere();

    void update();
    void addParts(int, int);
    void updateSize();
    void setPos(COORD pos);

    void init() {
        level = 1;
        stage = 1;
        stability = 50; 
        partA = 20, partB = 20, partC = 20;  
        exp = 0;  
        counterM = 0;
        dmtr = 8;
    }

    int level;
    int stage;  // current stage
    int stability;  // current sphere stability
    int partA, partB, partC;  // parts of the sphere
    int exp;  // experience
    int counterM;

    COORD position;
    short dmtr;  // sphere diameter
    short maxDmtr[3] = {16, 25, 30};  // max diameter of different spheres
    shapes::Ellipse SPR1;  // 
    shapes::Ellipse SPR2;  // sphere figures
    shapes::Ellipse SPR3;  //
    int ids[3];  // speheres id on the layout
    wchar_t fill[3] = { L'M', L'X', L'&' };  // sphere filling
} sphere;

struct UI {
    UI() = default;

    void update();
    void init();

    shapes::Sprite SPR;
    shapes::Sprite TXT;
} ui;

struct Particle : public shapes::Figure { 
    Particle(); 

    void update(Particle*);
    void spawn();
    void draw(Screen* screen, COORD cameraPOS);
    void collapse(Particle*);

    static constexpr size_t maxW = 11, maxH = 11;
    static int defaultV;

    COORD position;
    COORD vector;
    COORD vectorU = {0, 0};
    COORD maxC, minC;
    COORD sizeP;
    wchar_t type;
    wchar_t map[maxH][maxW];
    long long unsigned int counter = 0;
    int velocity;
    int volume;
    int protectC = 50;
    bool speedUp = false;
};

int Particle::defaultV = 4;

struct Game {
    Game() {
        restart();
    }

    void restart() {
        box.setState(0);
        for (int p : idsP) 
            mainScene.disableFigure(p);
        idsP.clear();
        particles.clear();

        addParticle();
        addParticle();
        addParticle();
        addParticle();
        addParticle();
        addParticle();

        sphere.init();

        counter = 0;
        selected = nullptr;
    }

    static constexpr COORD surfaceSize = {100, 100};

    void addParticle() {
        Particle* p = new Particle();
        particles.push_back(p);
        idsP.push_back(mainScene.addFigure(p));
    }

    void selectParticle(int axis) {
        COORD posB = box.position;
        posB.X += box.size.X / 2;
        posB.Y += box.size.Y / 2;
        int compP = 0;
        Particle* ptrP = nullptr;

        for (Particle* p : particles) {
            if (p == selected) continue;
            if (ptrP == nullptr) ptrP = p;
            int currP;
            
            switch (axis) {
            case 1:
                currP = posB.Y - p->position.Y;
                break;
            case 2:
                currP = p->position.X - posB.X;
                break;
            case 3:
                currP = p->position.Y - posB.Y;
                break;
            case 4:
                currP = posB.X - p->position.X;
            }

            if (compP <= 0) compP = currP + 1;

            if (currP < compP && currP >= 0) {
                compP = currP;
                ptrP = p;
            }
        }

        selected = ptrP;
        box.setState(1);
    }

    void update() {
        int parts[3] = {0, 0, 0};
        for (Particle* p : particles) {
            ++counter;
            
            if ((p->speedUp || p->vectorU.X || p->vectorU.Y) && p != selected) {
                p->speedUp = false;
                p->vectorU = {0, 0};
            }

            p->update(p);
            if (p->type == L'Z' && (Layout::collides(&sphere.SPR2, p) || Layout::collides(&sphere.SPR1, p))) {
                sphere.stability -= p->volume * 2;
                p->collapse(p);
            }

            switch(p->type) {
            case L'@': ++parts[0];
                break;
            case L'8': ++parts[1];
                break;
            case L'¢': ++parts[2];
            }
        }

        // easter egg
        if (counter % 20 == 0 && easterSPR->getPos().X > -20) {
            easterSPR->movePos({-1, 0});
            if (counter % 240 == 0) {
                if (easterTxr == 1) {
                    easterTxr = 2;
                    easterSPR->setTexture(&easterEgg1TXR);
                } else {
                    easterTxr = 1;
                    easterSPR->setTexture(&easterEgg2TXR);
                }
            }
        }
        if (counter % ((easterR + 50) * 100) == 0) {
            easterSPR->setPos({Game::surfaceSize.X, 10});
        }

        // balancing particles
        if (!nextP) {
            int avg = (parts[0] + parts[1] + parts[2]) / 3 * 0.5;
            if (avg > parts[0]) nextP = 1;
            if (avg > parts[1]) nextP = 2;
            if (avg > parts[2]) nextP = 3;
        }


        // decrease sphere stability
        if (counter % 400 == 0) --sphere.stability;

        if (counter % 200 == 0 && sphere.stability < 40) 
            if (sphere.partA > 25 && sphere.partA < 55 && sphere.partB > 25 && sphere.partB < 55 && sphere.partC > 25 && sphere.partC < 55)
                sphere.stability += 2;

        // decrease sphere parts
        if (counter % 400 == 0) {
            switch (rand() % 3) {
            case 0: --sphere.partA;
                break;
            case 1: --sphere.partB;
                break;
            case 2: --sphere.partC;
            }
        }

        // pause game
        if (keys::up(VK_ESCAPE)) {
            gameSt = 2;
            switchedL = true;
        }

        if (sphere.stability >= 100) sphere.stability = 100;

        // moving particle
        if (box.state < 2) {
            if (keys::up(VK_UP)) selectParticle(1);
            else if (keys::up(VK_RIGHT)) selectParticle(2);
            else if (keys::up(VK_DOWN)) selectParticle(3);
            else if (keys::up(VK_LEFT)) selectParticle(4);
        } else {
            if (keys::down('W')) selected->vectorU.Y = -3;
            else if (keys::down('S')) selected->vectorU.Y = 3;
            else selected->vectorU.Y = 0;

            if (keys::down('A')) selected->vectorU.X = -3;
            else if (keys::down('D')) selected->vectorU.X = 3;
            else selected->vectorU.X = 0;

            if (keys::up('E')) {
                if (Layout::collides(&sphere.SPR2, selected) || Layout::collides(&sphere.SPR1, selected)) {
                    int type;
                    switch (selected->type) {
                    case L'@': type = 1;
                        break;
                    case L'8': type = 2;
                        break;
                    case L'¢': type = 3;
                    }

                    sphere.addParts(type, selected->volume);
                    selected->collapse(selected);
                    sphere.stability += selected->volume / 2;
                    sphere.exp += selected->volume;
                }
            }

            // next stage
            if (sphere.exp >= 120) {
                sphere.stage += 1;
                sphere.exp = 0;
                Particle::defaultV = 4 + sphere.stage / 2;
                sphere.dmtr += sphere.stage * 1.5;
                addParticle();
            }

            selected->speedUp = keys::down(VK_SPACE);
        }

        // lose
        if (sphere.stability < 0 || (sphere.partA > 80 || sphere.partB > 80 || sphere.partC > 80) || (sphere.partA < 10 || sphere.partB < 10 || sphere.partC < 10)) {
            gameSt = 3;
            switchedL = true;
            pb = sphere.stage > pb ? sphere.stage : pb;
            return;
        } 

        // win
        if (sphere.dmtr > sphere.maxDmtr[2]) { 
            gameSt = 4;
            switchedL = true;
            pb = sphere.stage;
            return;
        }

        // box
        if (selected != nullptr) {
            if (keys::up(VK_RETURN)) box.setState(box.state == 1 ? 2 : 1);
            box.setSize({short (selected->sizeP.X + 3), short (selected->sizeP.Y + 3)});
            box.setPos({short (selected->position.X - 2), short (selected->position.Y - 2)});
        }

        sphere.update();
        box.draw();
        mainScene.draw(false);

        // UI
        string stageStr = to_string(sphere.stage);
        for (int x = 0; x < stageStr.length(); ++x) 
            screen.setPX_s({short (6 + x), 0}, stageStr[x]);

        short fromX = surfaceSize.X - 13;
        for (int x = 0; x <= sphere.stability / 10; ++x) 
            screen.setPX_s({short (fromX + x), 0}, L'|');

        for (int x = 0; x < sphere.exp / 10; ++x) 
            screen.setPX_s({short (fromX + x), 2}, L'*');

        short partAS = round(sphere.partA / 10.0f);
        short partBS = round(sphere.partB / 10.0f);
        short partCS = round(sphere.partC / 10.0f);

        for (int x = 0; x < partAS; ++x)
            screen.setPX_s({short (fromX + x), 1}, L'A');
        for (int x = 0; x < partBS; ++x)
            screen.setPX_s({short (fromX + partAS + x), 1}, L'B');
        for (int x = 0; x < partCS; ++x)
            screen.setPX_s({short (fromX + partAS + partBS + x), 1}, L'C');

        mainUI.draw();
    }

    Particle* selected = nullptr;
    vector<Particle*> particles;
    vector<int> idsP;
    int nextP = 0;
    int easterR = rand() % 50;
    int easterId;
    shapes::Sprite* easterSPR;
    int easterTxr = 1;

    long long unsigned int counter = 0;
} game;

//=========== UI CLASS ============

void UI::init() {
    mainUI.addFigure(&SPR);
    SPR.setTexture(&uiTXR);
    SPR.setSize(uiTXR.size);
    SPR.setPos({short (Game::surfaceSize.X - uiTXR.size.X), 0});

    mainUI.addFigure(&TXT);
    TXT.setTexture(&uiTxtTXR);
    TXT.setSize(uiTxtTXR.size);
}

void UI::update() {

}

//=========== SPHERE CLASS ============

Sphere::Sphere() {
    COORD sSize = Game::surfaceSize;

    ids[2] = mainScene.addFigure(&SPR3);
    ids[1] = mainScene.addFigure(&SPR2);
    ids[0] = mainScene.addFigure(&SPR1);
    SPR3.setFilling(fill[2]);
    SPR2.setFilling(fill[1]);
    SPR1.setFilling(fill[0]);

    SPR1.setSize({dmtr, dmtr});
    // center
    setPos({short (sSize.X / 2), short (sSize.Y / 2)});

    mainScene.disableFigure(ids[1]);
    mainScene.disableFigure(ids[2]);
}

void Sphere::addParts(int type, int volume) {
    int carry;
    int *a, *b, *c;
    switch(type) {
        case 1: 
            a = &partA;
            b = &partB;
            c = &partC;
            break;
        case 2: 
            a = &partB;
            b = &partA;
            c = &partC;
            break;
        case 3: 
            a = &partC;
            b = &partB;
            c = &partA;
    }

    *a += volume;
    carry = *a + *b + *c - 120;
    carry = carry > 0 ? carry : 0;
    *b -= carry / 2;
    *c -= carry / 2;
}

void Sphere::update() {
    if (dmtr > maxDmtr[1]) level = 3;
    else if (dmtr > maxDmtr[0]) level = 2;
    updateSize();

    if (counterM++ > stability / 10) {
        counterM = 0;

        // random sphere movement + forcing to be near center
        unsigned char r1 = rand() % 256;
        unsigned char r2 = r1 >> 4;
        COORD newPos;
        COORD wDir;
        wDir.X = position.X - Game::surfaceSize.X / 2 > 0 ? -1 : 1;
        wDir.Y = position.Y - Game::surfaceSize.Y / 2 > 0 ? -1 : 1;

        newPos.X = r1 & 0b1000 ? 2 : 1;
        newPos.Y = r2 & 0b1000 ? 2 : 1;
        newPos.X *= r1 & 0b0011 ? r1 & 0b0100 ? 1 : -1 : wDir.X;
        newPos.Y *= r2 & 0b0011 ? r2 & 0b0100 ? 1 : -1 : wDir.Y;
        newPos.X += position.X + dmtr / 2;
        newPos.Y += position.Y + dmtr / 2;

        setPos(newPos);
    }
}

void Sphere::updateSize() {
    short tempD[2] = {dmtr, dmtr};
    switch(level) {
    case 3:
        SPR3.setSize({dmtr, dmtr});
        mainScene.enableFigure(ids[2]);

        tempD[1] = maxDmtr[1];
    case 2:
        SPR2.setSize({tempD[1], tempD[1]});
        mainScene.enableFigure(ids[1]);

        tempD[0] = maxDmtr[0];
    case 1:
        SPR1.setSize({tempD[0], tempD[0]});
    }
}


void Sphere::setPos(COORD pos) {
    // offsetting spheres
    position = {short (pos.X - dmtr / 2), short (pos.Y - dmtr / 2)};
    short tempOff[2] = {0, 0};
    switch(level) {
    case 3:
        tempOff[1] = (dmtr - maxDmtr[1]) / 1.5;
        SPR3.setPos(position);
    case 2:
        tempOff[0] = (dmtr - maxDmtr[0]) / 2;
        SPR2.setPos({short (position.X + tempOff[1]), short (position.Y + tempOff[1])});
    case 1:
        SPR1.setPos({short (position.X + tempOff[0]), short (position.Y + tempOff[0])});
    }
}


//=========== PARTICLE CLASS ============


Particle::Particle() {
    counter = rand() % 50;
    spawn();
}

void Particle::update(Particle* thisP) {
    counter++;
    int tVel = velocity / (speedUp ? 2 : 1);

    if (counter % (velocity / abs(vector.X)) == 0) position.X += vector.X > 0 ? 1 : -1;
    if (counter % (velocity / abs(vector.Y)) == 0) position.Y += vector.Y > 0 ? 1 : -1;
    if (vectorU.X && counter % (tVel / abs(vectorU.X)) == 0) position.X += vectorU.X > 0 ? 1 : -1;
    if (vectorU.Y && counter % (tVel / abs(vectorU.Y)) == 0) position.Y += vectorU.Y > 0 ? 1 : -1;

    if (counter % 200 == 0) {
        position.X += (rand() % 3) - 1;
        position.Y += (rand() % 3) - 1;
    }

    // losing volume when speeding up
    if (speedUp && counter % 50 == 0) {
        --volume;

        for (short x = minC.X; x <= maxC.X; ++x) {
            if (false) bk: break;
            for (short y = minC.Y; y <= maxC.Y; ++y) {
                if (map[y][x] != L' ') {
                    map[y][x] = L' ';
                    goto bk;
                }
            }
        }
    }
    
    // respawning each time when leaves screen
    if (volume < 1 || !protectC && position.X + 5 < 0 || position.X > Game::surfaceSize.X || position.Y + 5 < 0 || position.Y > Game::surfaceSize.Y) {
        collapse(thisP);
        return;
    }

    setPos(position);
    if (protectC) --protectC;
}

void Particle::collapse(Particle* thisP) {
    spawn();
    if (game.selected == thisP) box.setState(0);
}

void Particle::spawn() {
    volume = defaultV;
    speedUp = false;
    vectorU = {0, 0};

    // setting type
    unsigned char r = game.nextP ? game.nextP : rand() % (sphere.level + 3);

    switch (r) {
    case 5:
    case 4:
    case 0:
        type = L'Z';
        velocity = 15;
        break;
    case 1:
        type = L'@';
        velocity = 8;
        break;
    case 2:
        type = L'8';
        velocity = 10;
        break;
    case 3:
        type = L'¢';
        velocity = 13;
    }

    game.nextP = 0;

    // settign position and vector
    r = rand() % 256;
    short X = Game::surfaceSize.X, Y = Game::surfaceSize.Y;
    
    // comparing 8th bit
    if (r & 0b10000000) {
        position.X = r % 8 * X / 8 + X / 16;
        // comparing 7th bit
        position.Y = r & 0b01000000 ? Y : -5;
        vector.X = r & 0b100 ? -1 : 1;
        vector.Y = r & 0b0100000 ? -1 : 1;
    } else {
        position.Y = r % 8 * Y / 8 + Y / 16;
        // comparing 7th bit
        position.X = r & 0b01000000 ? X : -5;
        vector.Y = r & 0b100 ? -1 : 1;
        vector.X = r & 0b0100000 ? -1 : 1;
    }
    
    // comparing 6th and 5th bits
    if (r ^ 0b00100000 & 0b00110000) vector.X *= 2;  // xx01xxxx
    if (r ^ 0b00010000 & 0b00110000) vector.Y *= 2;  // xx10xxxx

    // clearing map
    for (int y = 0; y < maxH; ++y) for(int x = 0; x < maxW; ++x) map[y][x] = L' ';
    short midH = maxH / 2, midW = maxW / 2;
    map[midH][midW] = type;

    // creating particle 
    maxC = {midW, midH}, minC = {midW, midH};
    for (int c = 1; c < volume; ++c) {
        r = rand() % 8;
        COORD dir;

        switch (r) {
        case 0: dir = {0, -1};
            break;
        case 1: dir = {1, -1};
            break;
        case 2: dir = {1, 0};
            break;
        case 3: dir = {1, 1};
            break;
        case 4: dir = {0, 1};
            break;
        case 5: dir = {-1, 1};
            break;
        case 6: dir = {-1, 0};
            break;
        case 7: dir = {-1, -1};
        }

        short x = midW, y = midH;
        for (; y >= 0 && y < maxH && x >= 0 && x < maxW && map[y][x] != L' '; x += dir.X, y += dir.Y);
        map[y][x] = type;

        if (x > maxC.X) maxC.X = x;
        else if (x < minC.X) minC.X = x;
        if (y > maxC.Y) maxC.Y = y;
        else if (y < minC.Y) minC.Y = y;
    }

    velocity *= sqrt(volume);

    size.X = maxC.X - minC.X + 1;
    size.Y = maxC.Y - minC.Y + 1;
    sizeP = size;
}

void Particle::draw(Screen* screen, COORD cameraPOS) {
    wchar_t curr;
    short startY = position.Y - cameraPOS.Y;
    short startX = position.X - cameraPOS.X;
    for (short y = 0; y < size.Y; ++y) {
        for (short x = 0; x < size.X; ++x) {
            curr = map[minC.Y + y][minC.X + x];
            if (curr != L' ') screen->setPX_s({short (startX + x), short (startY + y)}, curr);
        }
    }
}

//===================================