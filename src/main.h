



typedef enum _SysMode {
    MODE_INITIATION = 0, 
    MODE_SAFE, 
    MODE_RECOVERY, 
    MODE_STANDBY,
    MODE_PAYLOAD, 
    MODE_TRANSMISSION
} SysMode; 

// Access current state from other files. 
SysMode sys_get_current_mode(void); 

