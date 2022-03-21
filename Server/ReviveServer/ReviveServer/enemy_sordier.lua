enemy_id = 99999;
see_range=5;
enemy_x = 0;
enemy_y = 0;
enemy_z = 0;

enemy_maxhp=0;
enemy_hp=0;
enemy_damege = 0;
enemy_type = 0;

function initializEnemy(id, x, y, z, hp, damege, type)
	enemy_id = id;
	enemy_x = x;
	enemy_y = y;
	enemy_z = z;
	enemy_maxhp=hp;
	enemy_damege = damege;
	enemy_type = type;
end


function event_agro_fallow( player )
   player_x = API_get_x(player);
   player_y = API_get_y(player);
   nowx = API_get_x(myid);
   nowy = API_get_y(myid);
   if(player_x <= nowx +math.floor(range/2) and player_x > nowx -math.floor(range/2)) then
		if(player_y <= nowy + math.floor(range/2) and player_y > nowy -math.floor(range/2)) then
        
		end
	end
end


function event_test(npc_id )
	nowx=API_get_x(npc_id);
	nowy=API_get_y(npc_id);

end
