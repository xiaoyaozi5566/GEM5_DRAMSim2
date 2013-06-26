#ifndef __ENUM__ArmMachineType__
#define __ENUM__ArmMachineType__

namespace Enums {
    enum ArmMachineType {
        RealView_EB = 827,
        RealView_PBX = 1901,
        VExpress_CA9 = 2272,
        VExpress_ELT = 2272,
        VExpress_EMM = 2272,
        Num_ArmMachineType = 5
    };
extern const char *ArmMachineTypeStrings[Num_ArmMachineType];
}

#endif // __ENUM__ArmMachineType__
