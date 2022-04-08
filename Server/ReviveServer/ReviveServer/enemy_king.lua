enemy_id = 99999;
see_range=900;
attack_range=350;

enemy_x = 0;
enemy_y = 0;
enemy_z = 0;

enemy_maxhp=0;
enemy_hp=0;
enemy_damege = 0;
enemy_type = 0;

now_x=0.1;
now_y=0.1;
--state 1:idle, 2:move, 3: attack, 4:hit, 5:dead 
--table써서 switch문 만들기
function initializEnemy(id, x, y, z, hp, damege, type)
	enemy_id = id;
	enemy_x = x;
	enemy_y = y;
	enemy_z = z;
	enemy_maxhp=hp;
	enemy_damege = damege;
	enemy_type = type;
end

--API로 플레이어, 기지거리 확인하기
--가장 가까운 id 반환
--target_id설정 하는 API호출
function event_agro_fallow( player )
   player_x = API_get_x(player);
   player_y = API_get_y(player);
   nowx = API_get_x(myid);
   nowy = API_get_y(myid);
   
end


function event_test(npc_id)
	now_x = API_get_x(npc_id);
    now_y = API_get_y(npc_id);
	API_test_lua(now_x,now_y);
end

function state_machine()

end