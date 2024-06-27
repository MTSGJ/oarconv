default
{
    state_entry()
    {
        llSetTimerEvent(5.0);
    }

    timer()
    {
        vector sun = llGetSunDirection();
        if(sun.z > 0.10)
        {
            llSetPrimitiveParams([PRIM_MATERIAL, PRIM_MATERIAL_GLASS, PRIM_FULLBRIGHT, ALL_SIDES, FALSE, PRIM_POINT_LIGHT, FALSE,ZERO_VECTOR, 0.0, 0.0, 0.0]);
            llParticleSystem([]);
        }
        else
        {
            llSetPrimitiveParams([PRIM_MATERIAL, PRIM_MATERIAL_LIGHT, PRIM_FULLBRIGHT, ALL_SIDES, TRUE, PRIM_POINT_LIGHT, TRUE,<1,1,1>, 1.0, 12.0, 0.0]);
            llParticleSystem([PSYS_SRC_PATTERN, PSYS_SRC_PATTERN_ANGLE_CONE,
                            PSYS_PART_FLAGS, PSYS_PART_EMISSIVE_MASK|PSYS_PART_FOLLOW_SRC_MASK|PSYS_PART_INTERP_COLOR_MASK|PSYS_PART_INTERP_SCALE_MASK,
                            PSYS_PART_START_ALPHA, 0.8,
                            PSYS_PART_END_ALPHA, 0.0,
                            PSYS_PART_START_SCALE, llGetScale(),
                            PSYS_PART_END_SCALE, 2.0 * llGetScale(),
                            PSYS_PART_MAX_AGE, 0.75,
                            PSYS_SRC_BURST_PART_COUNT, 1,
                            PSYS_SRC_BURST_RATE, 0.05,
                            PSYS_SRC_ANGLE_BEGIN, 0.0,
                            PSYS_SRC_ANGLE_END, PI,
                            PSYS_SRC_ACCEL, ZERO_VECTOR,
                            PSYS_SRC_BURST_SPEED_MAX, 0.0,
                            PSYS_SRC_BURST_SPEED_MIN, 0.0]);
        }
    }
}
