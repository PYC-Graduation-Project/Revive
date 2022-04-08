enemy_id = 99999;
see_range=900;
attack_range=1700;
enemy_x = 0;
enemy_y = 0;
enemy_z = 0;

enemy_maxhp=0;
enemy_hp=0;
enemy_damege = 0;
enemy_type = 0;

now_x=0.1;
now_y=0.1;

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
   now_x = API_get_x(myid);
   now_y = API_get_y(myid);
   if(player_x <= nowx +math.floor(range/2) and player_x > nowx -math.floor(range/2)) then
		if(player_y <= nowy + math.floor(range/2) and player_y > nowy -math.floor(range/2)) then
        
		end
	end
end


function event_test(npc_id)
	
	 now_x = API_get_x(npc_id);
	 now_y = API_get_y(npc_id);
	API_test_lua(now_x,now_y);
end				 
