vector target = <0,0.4,0.3>;          //sit offset
vector target_rotation = <0,0,90>;      //adjust to suit your needs. in X/Y/Z degrees

default
{
    state_entry()
    {
       
        llSitTarget( target, llEuler2Rot ( (target_rotation * DEG_TO_RAD) ) );
}
}

