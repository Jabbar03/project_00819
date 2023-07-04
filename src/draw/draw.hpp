#include <ncurses.h>

#include "screen.hpp"

#include "../game/statistics.hpp"
#include "../maps/map.hpp"
#include "../maps/chunk.hpp"
#include "../maps/platform.hpp"
#include "../physics/point.hpp"
#include "../entity/entity.hpp"
#include "../entity/enemy.hpp"
#include "../entity/coin.hpp"
#include "../entity/bullet.hpp"

#include "../../etc/nostd/vector.hpp"
#include "../../etc/nostd/string.hpp"


#define OFFSET 42

#ifndef DRAW_DRAW
#define DRAW_DRAW

class Draw : public Screen{
	private:
		void drawPlatform(nostd::vector<Platform> plat);

        int centerX(nostd::string text);
        int centerX(const char* text);

	public:
		void drawText(int posY, int posX, nostd::string s);
		void drawText(int posY, int posX, const char* t);
		void drawText(int posY, int posX, const char s);

		void drawCenterText(int posY, nostd::string s);
		void drawCenterText(int posY, const char* t);

		void drawUpperText(int posY, int posX, nostd::string s);
		void attrOff(int c_pair);
		void drawBox();
		void attrOn(int c_pair);
		void clearWindow();
		void drawMap(Map map, int nChunck);
		void drawPlayer(phy::Point p);

		void drawSquareAround(nostd::string s, int posY, int posX);
        void drawCenterSquareAround(nostd::string s, int posY);
                                                               //
		void drawRectagle(int posY, int posX, int length, int width);


		int center(nostd::string t);
		Draw newSubWindow(int height, int width, int posY, int posX);
		Draw newWindow(int height, int width, int posY, int posX);
		void setWin(WINDOW *win);

		void drawEntity(Entity entity);
		void drawEntity(Enemy enemy);
		void drawEntity(Coin coint);
		void drawEntity(Bullet bullet);

        void updateStats(Statistics stats);
        void deleteStats();
};
#endif
