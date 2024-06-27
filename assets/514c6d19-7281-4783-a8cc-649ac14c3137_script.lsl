vector axis = <0,0,-1>;
float spinrate = 0.0234375;
float gain = 0.5;

default {
    state_entry() {
        llTargetOmega(axis, spinrate, gain);
    }
    on_rez(integer param) {
        llTargetOmega(axis, spinrate, gain);
    }
}