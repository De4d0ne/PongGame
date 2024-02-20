#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define pressed(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_pos_x = 0.f;
float player_pos_y = 0, f;

internal void 
simulate_game(Input* input, float delta_time)
{
	clear_screen(0x00B0000);

	float speed = 50.f;

	if (is_down(BUTTON_UP))
	{
		player_pos_y += speed * delta_time;
	}
	if (is_down(BUTTON_DOWN))
	{
		player_pos_y -= speed * delta_time;
	}
	if (is_down(BUTTON_RIGHT))
	{
		player_pos_x += speed * delta_time;
	}
	if (is_down(BUTTON_LEFT))
	{
		player_pos_x -= speed * delta_time;
	}

	draw_rect(player_pos_x, player_pos_y, 1, 1, 0xffffff);
}