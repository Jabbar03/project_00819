#include "manager.hpp"

Manager::Manager (Map map)
    : Global_Entities(0), Global_Coins(0), Global_Enemies(0), current_chunk(-1)
{
    this->map = map;
    this->seed = map.getSeed();
}

void Manager::set_chunk(int Chunk, Map map)
{
    if(this->current_chunk < Chunk) {
        this->Enemies.push_back(nullptr);
        this->Coins.push_back(nullptr);

        int number_of_enemies = map.getEnemies(Chunk);
        for(int i = 0; i < number_of_enemies; i++) {
            this->add_enemy(Chunk, 
                    EnemyType[Random::generateEnemyType(this->seed, Chunk, i) - 1], 
                    Random::generateEnemyPosition(map, Chunk, 
                        this->get_all_entities_positions_in_chunk(Chunk)), i%2 == 0);
        }

        int number_of_coins = map.getCoins(Chunk);
        for(int i = 0; i < number_of_coins; i++)
            this->add_coin(Chunk, 
                    CoinType[Random::generateCoinType(this->seed, Chunk, i) - 1], 
                    Random::generateCoinPosition(map, Chunk, 
                        this->get_all_entities_positions_in_chunk(Chunk)));
    }
    this->current_chunk = Chunk;
}

void Manager::add_enemy(int Chunk, Enemy enemy, phy::Point p, bool dir)
{
    enemy.set_point(p);
    enemy.set_direction(dir);

    head_insert(Chunk, enemy);
    this->Global_Entities++;
    this->Global_Enemies++;
}

void Manager::add_coin(int Chunk, Coin coin, phy::Point p)
{
    coin.set_point(p);

    head_insert(Chunk, coin);
    this->Global_Entities++;
    this->Global_Coins++;
}

void Manager::head_insert(int Chunk, Enemy enemy)
{
    if(this->Enemies[Chunk] == NULL) {
        pnemici p = new nemici;
        p->next = NULL;
        p->val = enemy;
        this->Enemies[Chunk] = p;
    } else {
        pnemici p = new nemici;
        p->next = this->Enemies[Chunk];
        p->val = enemy;
        this->Enemies[Chunk] = p;
    }
}

void Manager::head_insert(int Chunk, Coin coin)
{
    if(this->Coins[Chunk] == NULL) {
        pmonete p = new monete;
        p->next = NULL;
        p->val = coin;
        this->Coins[Chunk] = p;
    } else {
        pmonete p = new monete;
        p->next = this->Coins[Chunk];
        p->val = coin;
        this->Coins[Chunk] = p;
    }
}

/* Da riguardare la meccanica di raccolta e uccisione */

// void Manager::kill_entity(int Chunk, Enemy enemy)
// {
//     pnemici tmp = this->Enemies[Chunk];
//     while (tmp != NULL || enemy.get_point() == tmp->val.get_point())
//         tmp->val.set_state(false);
//
//     this->Global_Enemies--;
//     this->Global_Entities--;
// }

// void Manager::collect_coin(int Chunk, Coin coin)
// {
//     pmonete tmp = this->Coins[Chunk];
//     while (tmp != NULL || coin.get_point() == tmp->val.get_point())
//         tmp->val.set_state(true);
//
//     this->Global_Coins--;
//     this->Global_Entities--;
// }

//time is in dec sec (sec*10^-1)
void Manager::move_enemies(int& time)
{
    if(time == 0) {
        pnemici tmp = this->Enemies[this->current_chunk];
        Chunk chunk = map.get_chunk(this->current_chunk);
        while(tmp != NULL) {
            if(tmp->val.isItAlive()) {
                if(tmp->val.canMove(chunk)) {
                    if(tmp->val.get_direction()) 
                        tmp->val.set_point(tmp->val.get_point() + phy::Point(1,0));
                    else 
                        tmp->val.set_point(tmp->val.get_point() + phy::Point(-1,0));
                }
                else 
                    tmp->val.set_direction(!tmp->val.get_direction()); 
            }
            tmp = tmp->next;
        }
    } 
}

void Manager::draw_entities(Draw screen)
{
    pnemici p = this->Enemies[this->current_chunk];
    while(p != NULL) {
        screen.drawEntity(p->val);
        p = p->next;
    }

    pmonete q = this->Coins[this->current_chunk];
    while(q != NULL) {
        screen.drawEntity(q->val);
        q = q->next;
    }
}


bool Manager::is_there_an_entity_in_platform(Map map, int Chunk, int plat)
{
    bool check = false;
    Platform platform = map.get_chunk(Chunk).get_platforms()[plat];

    int start = platform.get_position().get_xPosition();
    int end = platform.get_position().get_xPosition() + platform.get_length();
    pnemici p = get_all_enemies_in_chunk(Chunk);
    pmonete q = get_all_coins_in_chunk(Chunk);

    while(p != NULL && !check) {
        if(p->val.get_y() == platform.get_position().get_yPosition()) {
            if(p->val.get_x() >= start && p->val.get_x() <= end) {
                check = true;
            }
        }
        p = p->next;
    }

    while(q != NULL && !check) {
        if(q->val.get_y() == platform.get_position().get_yPosition()) {
            if(q->val.get_x() >= start && q->val.get_x() <= end) {
                check = true;
            }
        }
        q = q->next;
    }

    return(check);
}

bool Manager::is_there_an_entity_in_point(int Chunk, phy::Point point)
{
    bool check = false;

    pnemici p = get_all_enemies_in_chunk(Chunk);
    pmonete q = get_all_coins_in_chunk(Chunk);

    while(p != NULL && !check) {
        if(p->val.get_y() == point.get_yPosition() && p->val.get_x() == point.get_xPosition()) {
            check = true;
        }
        p = p->next;
    }

    while(q != NULL && !check) {
        if(q->val.get_y() == point.get_yPosition() && q->val.get_x() == point.get_xPosition()) {
            check = true;
        }
        q = q->next;
    }

    return(check);
}

pnemici Manager::get_all_enemies_in_chunk(int Chunk)
{
    pnemici p = this->Enemies[Chunk];
    return(p);
}

pmonete Manager::get_all_coins_in_chunk(int Chunk)
{
    pmonete p = this->Coins[Chunk];
    return(p);
}

nostd::vector<phy::Point> Manager::get_all_entities_positions_in_chunk(int Chunk)
{
    nostd::vector<phy::Point> v;
    pnemici p = this->Enemies[Chunk];
    pmonete q = this->Coins[Chunk];

    while (p != nullptr) {
        v.push_back(p->val.get_point());
        p = p->next;
    }

    while (q != nullptr) {
        v.push_back(q->val.get_point());
        q = q->next;
    }
    return(v);
}
//ONLY FOR DEBUGING
// void Manager::print_enemy_list()
// {
//     pnemici p = this->Enemies[this->current_chunk];
//     int c = 0;
//     while (p != nullptr) {
//         deb::debug(p->val.get_id(), "type");
//         deb::debug(p->val.get_point(), "current point");
//         p = p->next;
//         c++;
//     }
//     deb::debug(c, "number of enemies");
// }
