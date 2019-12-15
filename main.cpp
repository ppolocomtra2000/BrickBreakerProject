#include "BrickBreaker.h"
#include "menu.h"
#include <ctime>
#include <utility>

BrickBreaker game;
bool game_active = true;

// nút để chọn lựa chọn tiếp theo
const int choice_next[2] = {VK_DOWN, 'S'};
// nút để chọn lựa chọn trước đó
const int choice_prev[2] = {VK_UP, 'W'};

void menu::DrawMenu() {
    // vị trí của tiêu đề menu
    int x = game.RoughXCenter();
    int y = 90;
    // vẽ tiêu đề menu, sau đó tăng tọa độ y lên để vẽ tiếp các lựa chọn
    game.DrawString(x, y+=10, message, message_color);
    // vẽ các lựa chọn
    // nếu đố là lựa chọn được chọn thì có dấu ‘<’ để biểu thị
    game.DrawString(x, y+=10,
                    current_choice%3 == 0? choice1 + "<" : choice1, choice_color);
    game.DrawString(x, y+=10,
                    current_choice%3 == 1? choice2 + "<" : choice2, choice_color);
    game.DrawString(x, y+=10,
                    current_choice%3 == 2? choice3 + "<" : choice3, choice_color);
    // nếu ấn một trong các nút chọn tiếp theo thì tăng current choice
    for (int i : choice_next)
        if(game.GetKey(i).bPressed)
            current_choice++;
    // tương tự
    for (int i : choice_prev)
        if(game.GetKey(i).bPressed)
            current_choice--;
    // làm tròn current_choice để tạo hiệu ứng vòng lặp của các lựa chọn
    current_choice = current_choice%3 + 3;

    // Nếu ấn enter thì thực hiện hành động của lựa chọn được chọn
    if(game.GetKey('\r').bPressed){
        if(current_choice%3 == 0)
            action1();
        else if(current_choice%3 == 1)
            action2();
        else action3();
    }
}

menu::menu(string message, string choice1, string choice2, string choice3) : message(std::move(message)),
                                                                                 choice1(std::move(choice1)),
                                                                                choice2(std::move(choice2)),
                                                                                choice3(std::move(choice3)),
                                                                                 message_color(FG_RED),
                                                                                 choice_color(FG_WHITE),
                                                                                 current_choice(0) {}


class type1_menu : public menu {
    void action1() override {
        // khởi tạo game mới
        game.OnUserCreate();
        // tắt menu để chơi game
        game.active_menu = nullptr;
    }
    void action2() override {
        // Nếu load game thành công thì mới tắt menu
        if (game.Load())
            game.active_menu = nullptr;
    }
    void action3() override {
        // gán để BrickBreaker biết lmà thoát game
        game_active = false;
    }
    void DrawMenu() override {
        // Vẽ trang trí cho game
        game.Fill(0, 0, game.ScreenWidth(), game.ScreenHeight(), L' ');
        game.Fill(0,100, 20, 110, PIXEL_SOLID,FG_CYAN);
        game.Fill(0,110, 20, 140, PIXEL_SOLID,FG_MAGENTA);
        menu::DrawMenu();
    }
public:
    type1_menu(string message, string choice1, string choice2, string choice3) : menu(std::move(message), std::move(choice1), std::move(choice2), std::move(choice3)) {}
} gameOverMenu{"game over", "replay", "load", "close"}, mainMenu{"bricks", "play", "load", "close"};

class type2_menu;
extern type2_menu save_success_menu;

class type2_menu : public menu {
    void action1() override {
        // tắt menu để tiếp tục chơi
        game.active_menu = nullptr;
    }
    void action2() override {
        // lưu game và hiển thị thông báo lưu thành công
        game.Save();
        game.active_menu = &save_success_menu;
    }
    void action3() override {
        // thoát game, không lưu
        game.active_menu = &mainMenu;
    }
public:
    type2_menu(string message, string choice1, string choice2, string choice3) : menu(std::move(message), std::move(choice1), std::move(choice2), std::move(choice3)) {
        message_color = FG_MAGENTA;
        choice_color = FG_CYAN;
    }
} pause_menu{"restarting... ready?", "ready!", "save game", "back"}, save_success_menu{"game saved", "play", "save again", "back"};

// vẽ pixel_glyph tương ứng với ký tự ch
void BrickBreaker::DrawGlyph(int &x, int y, char ch, short color) {
    // kiểm tra nếu chưa vẽ chữ cho ch thì báo lỗi
    if(!glyphs[ch]) {
        cout<<"Character not found: "<<ch<<endl;
        throw;
    }
    const pixel_glyph& glyph = *glyphs[ch];
    // Vẽ từng pixel của glyph
    for (int i = 0; i < glyph.width; ++i, ++x) {
        for (int dy = 0; dy < glyph.height; ++dy) {
            // Kiểm tra có nằm ngoài màn hình không
            Clip(x,y);
            // nếu giá trị khác 0 thì vẽ
            if(glyph.value[i + dy*glyph.width])
                // nếu giá trị là 1 thì vẽ kín, nếu là 2 thì vẽ 50%
                Draw(x,y+dy,glyph.value[i + dy*glyph.width]==2?'M':PIXEL_SOLID, color);
        }
    }
}
void BrickBreaker::DrawGlyph(int x, int y, const pixel_glyph& glyph, short color) {
    for (int i = 0; i < glyph.width; ++i, ++x) {
        for (int dy = 0; dy < glyph.height; ++dy) {
            Clip(x,y);
            if(glyph.value[i + dy*glyph.width])
                Draw(x,y+dy,glyph.value[i + dy*glyph.width]==2?'O':PIXEL_SOLID, color);
        }
    }
}

void BrickBreaker::DrawString(int x, int y, const string& str, short color) {
    for(auto ch:str)
        DrawGlyph(x,y,ch, color);
}

void BrickBreaker::ResetBall() {
    //khởi tạo các giá trị khởi đầu của quả bóng
    ball.x = 64.0f;
    ball.y = 96.0f;
    ball.speed = 40.0f;


    //Góc xác định hướng bật của quả bóng
    float fAngle = ((float)rand() / (float)(RAND_MAX)) * M_PI_2 - M_PI_4*3;


    //khởi tạo hướng
    ball.dx = cosf(fAngle);
    ball.dy = sinf(fAngle);
}

void BrickBreaker::RecreateMap() {
    cout<<"recreate map"<<endl;
    //tạo khung cho sân chơi
    hit_left = 0;
    map = "";
    switch(level){
        case 0:
            map += ".....................";
            map += ".s.........r.........";
            map += ".....................";
            map += ".00000000kkkk00000000";
            map += ".0------------------0";
            map += ".0-bbbbbbbbbbbbbbbb-0";
            map += ".0-bbbbbbbbbbbbbbbb-0";
            map += ".0-bbbbbbbbbbbbbbbb-0";
            map += ".0-bbbbbbbbbbbbbbbb-0";
            map += ".0-w-uw--w--w--wx-w-0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0##################0";
            map += ".....................";
            map += ".n.............d.....";
            map += ".....................";
            break;
        case 1:
            map += ".....................";
            map += ".s.........r.........";
            map += ".....................";
            map += ".00000000kkkk00000000";
            map += ".0------------------0";
            map += ".0-wbbbbw----wbbbbw-0";
            map += ".0bbbbbbb----bbbbbbb0";
            map += ".0bbbbbbb----bbbbbbb0";
            map += ".0-bbbbbb----bbbbbb-0";
            map += ".0-bbbbbb----bbbbbb-0";
            map += ".0-w-uw--w--w--wx-w-0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0##################0";
            map += ".....................";
            map += ".n.............d.....";
            map += ".....................";
            break;
        case 2:
            map += ".....................";
            map += ".n.............d.....";
            map += ".....................";
            map += ".00000000kkkk00000000";
            map += ".0------------------0";
            map += ".0-bbbbbb----bbbbbb-0";
            map += ".0-bbbbbb----bbbbbb-0";
            map += ".0-bbbbbbbbbbbbbbbb-0";
            map += ".0-bbbbbbbbbbbbbbbb-0";
            map += ".0-bbbbbbbbbbbbbbbb-0";
            map += ".0-00w00w0000w00w00-0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0------------------0";
            map += ".0##################0";
            map += ".....................";
            map += ".s.........r.........";
            map += ".....................";
            break;
    }
    level = (level+1)%3;
    for(auto& ch:map){
        if(ch=='b')
            ch = rand()%2 ? '1':rand()%10?'2':'3';
        if(ch=='w')
            ch = rand()%2 ? '-':'0';
        if(ch=='u')
            ch = rand()%3 ? '-':'u';
        if(ch=='x')
            ch = rand()%2 ? '-':'x';
        if(ch=='1')
            hit_left+=1;
        if(ch=='2')
            hit_left+=2;
        if(ch=='3')
            hit_left+=1;
    }
}

bool BrickBreaker::OnUserCreate()  {
    ifstream stream("game_data.txt");
    if(stream) {
        stream >> highscore;
        stream.close();
    } else highscore = 0;

    level = 0;
    RecreateMap();
    ResetBall();

    //khởi tạo các giá trị khởi đầu của ván hứng
    pad.pos = 64.0f;
    pad.speed = 50.0f;
    pad.width = 15;

    life = 3;
    score = 0;

    active_menu = &mainMenu;
    captured_screen = new CHAR_INFO[m_nScreenWidth*m_nScreenHeight];
    return true;
}
const pixel_glyph lf_up_glyph = pixel_glyph(new char[64]{
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,1,0,
        0,0,1,0,0,1,1,0,
        0,1,1,1,0,0,1,0,
        0,0,1,0,0,0,1,0,
        0,0,0,0,0,0,1,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
}, 8, 8);
const pixel_glyph double_score_glyph = pixel_glyph(new char[64]{
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,1,1,1,
        0,1,0,1,0,0,0,1,
        0,0,1,0,0,1,1,1,
        0,1,0,1,0,1,0,0,
        0,0,0,0,0,1,1,1,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
}, 8, 8);
const pixel_glyph keyhole_glyph = pixel_glyph(new char[64]{
        1,1,1,1,1,1,1,1,
        1,1,1,0,0,1,1,1,
        1,1,0,0,0,0,1,1,
        1,1,0,0,0,0,1,1,
        1,1,1,0,0,1,1,1,
        1,1,2,0,0,2,1,1,
        1,1,0,0,0,0,1,1,
        1,1,1,1,1,1,1,1,
}, 8, 8);
const pixel_glyph keyhole_open_glyph = pixel_glyph(new char[64]{
        1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,
}, 8, 8);
const pixel_glyph tnt_glyph = pixel_glyph(new char[64]{
        0,0,0,0,0,0,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,1,1,0,0,0,
        0,0,0,0,0,0,0,0,
}, 8, 8);
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
bool BrickBreaker::OnUserUpdate(float fElapsedTime) {
    // nếu đang mở menu thì update menu thay vì update game
    if (active_menu)
    {
        // tạo hiệu ứng dừng trò chơi cho menu pause
        for (int x = 0; x < m_nScreenWidth; x++)
            for (int y = 0; y < m_nScreenHeight; y++) {
                int i = y * m_nScreenWidth + x;
                m_bufScreen[m_nCurrentBuffer][i] = captured_screen[i];
            }
        // update menu
        active_menu->DrawMenu();
    } else {
        // xóa buffer để vẽ frame mới
        Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');
        // di chuyển ván hứng
        if (GetKey(VK_LEFT).bHeld || GetKey('A').bHeld)
            pad.pos = pad.pos - pad.speed * fElapsedTime;
        if (GetKey(VK_RIGHT).bHeld || GetKey('D').bHeld)
            pad.pos = pad.pos + pad.speed * fElapsedTime;
        // nếu bấm phím cách thì di chuyển tự động
        if (GetKey(' ').bHeld)
            pad.pos = pad.pos + pad.speed * fElapsedTime * sgn(ball.x - pad.pos);

        // không cho ván di chuyển ra khỏi sân chơi
        if (pad.pos - pad.width < Block * 2)
            pad.pos = Block * 2 + pad.width;
        if (pad.pos + pad.width > Block * (mapWidth - 1))
            pad.pos = Block * (mapWidth - 1) - pad.width;

        //xử lí va chạm
        //2 biến này để lưu giữ giá trị tọa độ cũ của quả bóng trên màn hình
        float fOldX = ball.x;
        float fOldY = ball.y;


        // tính vị trí mới dựa trên Elapsedtime và tốc độ bóng
        ball.x += ball.dx * fElapsedTime * ball.speed;
        ball.y += ball.dy * fElapsedTime * ball.speed;

        // tọa độ cũ trên map của banh
        int nCellOldX = (int) fOldX / Block;
        int nCellOldY = (int) fOldY / Block;
        // tọa độ mới trên map
        int nCellNewX = (int) ball.x / Block;
        int nCellNewY = (int) ball.y / Block;

        // va chạm ván hứng với ván hứng
        if (ball.y > (playHeight * Block - 4) && (ball.x > pad.pos - pad.width - 2) && (ball.x < pad.pos + pad.width + 2)) {
            //bóng bật lại
            ball.dy *= -1;
            ball.y = playHeight * Block - 4;
            //tăng tốc độ banh
            ball.speed *= 1.08f;
            // hiệu ứng độ cong của ván
            ball.dx = clamp(ball.dx + clamp((float) (ball.x - pad.pos) / pad.width, -1.0f, 1.0f) * 0.4f, -2.0f, 2.0f);
            //tăng tốc độ ván
            pad.speed = pad.speed * 1.1f;
        }
        // Giảm thời gian còn lại của powerup gấp đôi điểm
        double_score_left-=fElapsedTime;
        // chỉ rõ ra ô đó là ô nào trong sân chơi
        char &newCell = map[nCellNewY * mapWidth + nCellNewX];
        // xử lý va chạm với các loại ô
        if (newCell == 'u') {
            // ô powerup tăng 1 mạng
            newCell = '-';
            life++;;
        } else if (newCell == 'x') {
            // ô powerup nhân đôi điểm trong 10s
            newCell = '-';
            double_score_left = 10;
        } else if (newCell == 'o') {
            // cửa đã mở
            // sang màn mới
            RecreateMap();
            ResetBall();
        } else if(newCell == '#') {
            // ô chết, nếu không hứng được thì banh sẽ chạm vào ô này và mất mạng
            CaptureScreen();
            life--;
            // nếu hết mạng thì game over, nếu không thì bật menu pause
            if(life == 0) {
                // nếu đạt kỷ lục thì lưu lại kỷ lục đó
                if(score>highscore){
                    highscore = score;
                    ofstream stream("game_data.txt");
                    stream << highscore;
                    stream.close();
                    gameOverMenu.message = "new record!!";
                } else gameOverMenu.message = "game over";
                // hiện menu game over
                active_menu = &gameOverMenu;
            } else active_menu = &pause_menu;
            pad.speed = 50.0f;
            // set lại vị trí bóng và tốc độ như ban đầu
            ResetBall();
        } else if (newCell != '-')
        {
            // đối với các ô còn lại
            // gây sát thương cho ô đó
            OnDamage(nCellNewX, nCellNewY);

            // gây sát thương xong thì cho banh bật lại
            if (nCellNewX != nCellOldX) {
                ball.dx = -1 * ball.dx;
                ball.x += ball.dx * fElapsedTime * ball.speed;
            }
            if (nCellNewY != nCellOldY) {
                ball.dy = -1 * ball.dy;
                ball.y += ball.dy * fElapsedTime * ball.speed;
            }

        }
        auto wall_color = FG_MAGENTA;
        auto death_color = FG_CYAN;
        auto moving_color = FG_CYAN;
        auto brick_color = FG_MAGENTA;
        //vẽ sân chơi, bóng, ván hứng
        for (int i = 0; i < mapHeight; i++)
        {
            for (int j = 0; j < mapWidth; j++)
            {
                int x;
                switch (map[i * mapWidth + j])
                {
                    case '#': // ô chết
                        FillCell(j,i,death_color);
                        break;
                    case '-': // sân chơi (ko có gì hết)
                        FillCell(j,i,FG_BLACK);
                        break;
                    case '0': // ô tường
                        FillCell(j,i,wall_color);
                        break;
                    case '1': // gach thường 1 máu
                        FillCell(j,i,brick_color-8, PIXEL_HALF);
                        break;
                    case '2': // gạch ống 2 máu
                        FillCell(j,i,brick_color, PIXEL_HALF);
                        break;
                    case '3': // bom
                        FillCell(j,i,brick_color -8, PIXEL_HALF);
                        DrawGlyph(j * Block, i * Block, tnt_glyph, brick_color);
                        break;
                    case 's': // ô hiển thị điểm
                        DrawString(j * Block, i * Block, "score " + to_string(score), double_score_left>0?FG_RED:FG_WHITE);
                        break;
                    case 'n': // ô hiển thị tên game
                        DrawString(j * Block, i * Block, "bricks", FG_RED);
                        break;
                    case 'd': // ô hiển thị số mạng
                        DrawString(j * Block, i * Block, "lives " + to_string(life), FG_WHITE);
                        break;
                    case 'r': // ô hiển thị kỷ lục
                        if(highscore) {
                            DrawString(j * Block, i * Block,"record ", FG_WHITE);
                            DrawString((j+6) * Block, i * Block,to_string(highscore), FG_RED);
                        }
                        break;
                    case 'u': // powerup 1 mạng
                        DrawGlyph(j * Block, i * Block, lf_up_glyph, FG_CYAN);
                        break;
                    case 'x': // powerup nhân điểm
                        DrawGlyph(j * Block, i * Block, double_score_glyph, FG_RED);
                        break;
                    case 'k': // ô cửa còn khóa
                        DrawGlyph(j * Block, i * Block, keyhole_glyph, wall_color);
                        break;
                    case 'o': // ô cửa đã mở
                        DrawGlyph(j * Block, i * Block, keyhole_open_glyph, wall_color);
                        break;
                }
            }
        }
        //vẽ banh
        FillCircle(ball.x, ball.y, 2.0f, PIXEL_SOLID, moving_color);
        //vẽ ván hứng
        DrawLine(pad.pos - pad.width, playHeight * Block - 2, pad.pos, playHeight * Block - 3, PIXEL_SOLID, moving_color);
        DrawLine(pad.pos, playHeight * Block - 3, pad.pos + pad.width, playHeight * Block - 2, PIXEL_SOLID, moving_color);
    }
    return game_active;
}
// gây sát thương cho ô
void BrickBreaker::OnDamage(int x, int y) {
    char& cell = map[y * mapWidth + x];
    // nếu là bom thì nổ và gây sát thương cho 8 ô xung quanh
    if(cell == '3') {
        // trở thành ô trống
        cell = '-';
        // nêu còn thời gian gấp đôi điểm thì được 200, nếu ko thì được 100 điểm
        score += double_score_left>0?200:100;
        // gây sát thương xung quanh
        OnDamage(x+1, y);
        OnDamage(x-1, y);
        OnDamage(x, y+1);
        OnDamage(x, y-1);
        OnDamage(x+1, y+1);
        OnDamage(x-1, y+1);
        OnDamage(x+1, y-1);
        OnDamage(x-1, y-1);
        // ghi nhớ đã phá được 1 ô
        hit_left--;
        // nếu đã phá hết thì mở khóa các cửa
        if(hit_left == 0) {
            for(auto& ch:map)
                if(ch=='k')
                    ch = 'o';
        }
    }
    if (cell == '1') {
        // được điểm
        score += double_score_left>0?200:100;
        // ghi nhớ đã phá được 1 ô
        hit_left--;
        if(hit_left == 0) {
            for(auto& ch:map)
                if(ch=='k')
                    ch = 'o';
        }
        // xuất hiện powerup theo ngẫu nhiên
        if(((float)rand() / (float)(RAND_MAX)) < x_spawn_chance)
            cell = 'x';
        else if(((float)rand() / (float)(RAND_MAX)) < u_spawn_chance)
            cell = 'u';
        // nếu xui thì biến mất trở lại ô trống
        else cell = '-';
    }
    if (cell == '2') {
        // mất 1 máu, trở về gạch thường
        cell = '1';
        // được điểm
        score += double_score_left>0?100:50;
        // không cần phải kiểm tra phá hết chưa, vì mình vẫn còn 1 máu chưa đc phá
        hit_left--;
    }
}

int BrickBreaker::RoughXCenter() {
    return ScreenWidth() / 8;
}

void BrickBreaker::Save() {
    ofstream stream("game.txt");
    stream << map<<endl << score<<" "<<life << " " << level <<" "<<double_score_left<<endl;
    stream << pad.pos <<" "<<pad.speed<<endl;
    stream << ball.x <<" "<<ball.y <<" "<<ball.dx <<" "<<ball.dy <<" "<<ball.speed<<endl;
    stream.close();
}

bool BrickBreaker::Load() {
    ifstream stream("game.txt");
    if(!stream)
        return false;
    stream >> map >> score >> life >> level >> double_score_left;
    stream >> pad.pos >>pad.speed;
    stream >> ball.x >>ball.y >>ball.dx >>ball.dy >>ball.speed;
    stream.close();
    return true;
}

void BrickBreaker::CaptureScreen() {
    for (int x = 0; x < m_nScreenWidth; x++)
        for (int y = 0; y < m_nScreenHeight; y++) {
            int i = y * m_nScreenWidth + x;
            captured_screen[i] = m_bufScreen[(m_nCurrentBuffer+1)%2][i];
        }
}

void BrickBreaker::FillCell(int x, int y, short color, wchar_t c) {
    Fill(x * Block, y * Block, (x + 1) * Block, (y + 1) * Block, c, color);
}

int main()
{
    srand(time(NULL));
    init_glyphs();
    game.ConstructConsole(176, 176, 4, 4);
    game.Start();
    return 0;
}
