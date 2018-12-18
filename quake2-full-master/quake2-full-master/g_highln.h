#define SWORD_NORMAL_DAMAGE 20
#define SWORD_DEATHMATCH_DAMAGE 20
#define SWORD_KICK 1200
#define SWORD_RANGE 35

void fire_sword(edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick)
{
	trace_t tr;             

	vec3_t          dir;
	vec3_t          forward;
	vec3_t          right;
	vec3_t          up;
	vec3_t          end;
	
	VectorMA(start, SWORD_RANGE, aimdir, end);
	tr = gi.trace(self->s.origin, NULL, NULL, end, self, MASK_SHOT);

	/*
	if (!(tr.fraction < 1.0))
	{
		vectoangles(aimdir, dir);
		AngleVectors(dir, forward, right, up);

		VectorMA(start, 8192, forward, end);
	}
	*/
	/*if (!self->groundentity)
	{
		gi.cprintf(ent, PRINT_CHAT, "Sword Swung While Jumping\n");
	}
	*/
	if (!((tr.surface) && (tr.surface->flags & SURF_SKY)))
	{
		if (tr.fraction < 1.0)
		{
			if (tr.ent->takedamage)
			{
				T_Damage(tr.ent, self, self, aimdir, tr.endpos, tr.plane.normal, damage, kick, 0, 0);

				gi.sound(self, CHAN_AUTO, gi.soundindex("misc/fhit3.wav"), 1, ATTN_NORM, 0);
			}
			else
			{
				gi.WriteByte(svc_temp_entity);
				gi.WriteByte(TE_SPARKS);
				gi.WritePosition(tr.endpos);
				gi.WriteDir(tr.plane.normal);
				gi.multicast(tr.endpos, MULTICAST_PVS);
				
				gi.sound(self, CHAN_AUTO, gi.soundindex("weapons/grenlb1b.wav"), 1, ATTN_NORM, 0);
			}
		}
	}
	return;
}	

void sword_attack(edict_t *ent, vec3_t g_offset, int damage)
{
	vec3_t  forward, right;
	vec3_t  start;
	vec3_t  offset;

	AngleVectors(ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, ent->viewheight - 8);
	VectorAdd(offset, g_offset, offset);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	fire_sword(ent, start, forward, damage, SWORD_KICK);

	
}

void Weapon_Sword_Fire(edict_t *ent)
{
	int damage;
	if (deathmatch->value)
		damage = SWORD_DEATHMATCH_DAMAGE;
	else
		damage = 20;
	
	sword_attack(ent, vec3_origin, damage);
	ent->client->ps.gunframe++;
	//gi.cprintf(ent, PRINT_CHAT, "Sword Swung Normally\n");
}

void Weapon_Sword(edict_t *ent)
{
	gi.cprintf(ent, PRINT_LOW, "Sword Currently Equipped\n");
	static int      pause_frames[] = { 19, 32, 0 };
	static int      fire_frames[] = { 5, 0 };

	Weapon_Generic(ent, 4, 8, 52, 55, pause_frames, fire_frames, Weapon_Sword_Fire);
}
