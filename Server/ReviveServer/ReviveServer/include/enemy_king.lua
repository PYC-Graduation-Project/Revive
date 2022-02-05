enemy_id = 99999;
see_range=5;
enemy_id = id;
enemy_x = 0;
enemy_y = 0;
enemy_level = 0;
enemy_maxhp=0;
enemy_hp=0;
enemy_damege = 0;
enemy_type = 0;

function initializEnemy(id, x, y, level, hp, exp, damege, type)
	enemy_id = id;
	enemy_x = x;
	enemy_y = y;
	enemy_level = level;
	enemy_maxhp=hp;
	enemy_damege = damege;
	enemy_type = type;
end



