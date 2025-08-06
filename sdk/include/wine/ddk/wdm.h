
#pragma once

typedef struct _KSYSTEM_TIME {
    ULONG LowPart;
    LONG High1Time;
    LONG High2Time;
} KSYSTEM_TIME, *PKSYSTEM_TIME;

#define PROCESSOR_FEATURE_MAX 64

typedef enum _ALTERNATIVE_ARCHITECTURE_TYPE
{
   StandardDesign,
   NEC98x86,
   EndAlternatives
} ALTERNATIVE_ARCHITECTURE_TYPE;

typedef enum _NT_PRODUCT_TYPE {
    NtProductWinNt = 1,
    NtProductLanManNt,
    NtProductServer
} NT_PRODUCT_TYPE, *PNT_PRODUCT_TYPE;

typedef struct _KUSER_SHARED_DATA {
    ULONG TickCountLowDeprecated;                          /* 0x000 */
    ULONG TickCountMultiplier;                             /* 0x004 */
    volatile KSYSTEM_TIME InterruptTime;                   /* 0x008 */
    volatile KSYSTEM_TIME SystemTime;                      /* 0x014 */
    volatile KSYSTEM_TIME TimeZoneBias;                    /* 0x020 */
    USHORT ImageNumberLow;                                 /* 0x02c */
    USHORT ImageNumberHigh;                                /* 0x02e */
    WCHAR NtSystemRoot[260];                               /* 0x030 */
    ULONG MaxStackTraceDepth;                              /* 0x238 */
    ULONG CryptoExponent;                                  /* 0x23c */
    ULONG TimeZoneId;                                      /* 0x240 */
    ULONG LargePageMinimum;                                /* 0x244 */
    ULONG AitSamplingValue;                                /* 0x248 */
    ULONG AppCompatFlag;                                   /* 0x24c */
    ULONGLONG RNGSeedVersion;                              /* 0x250 */
    ULONG GlobalValidationRunLevel;                        /* 0x258 */
    volatile ULONG TimeZoneBiasStamp;                      /* 0x25c */
    ULONG NtBuildNumber;                                   /* 0x260 */
    NT_PRODUCT_TYPE NtProductType;                         /* 0x264 */
    BOOLEAN ProductTypeIsValid;                            /* 0x268 */
    USHORT NativeProcessorArchitecture;                    /* 0x26a */
    ULONG NtMajorVersion;                                  /* 0x26c */
    ULONG NtMinorVersion;                                  /* 0x270 */
    BOOLEAN ProcessorFeatures[PROCESSOR_FEATURE_MAX];      /* 0x274 */
    ULONG Reserved1;                                       /* 0x2b4 */
    ULONG Reserved3;                                       /* 0x2b8 */
    volatile ULONG TimeSlip;                               /* 0x2bc */
    ALTERNATIVE_ARCHITECTURE_TYPE AlternativeArchitecture; /* 0x2c0 */
    ULONG BootId;                                          /* 0x2c4 */
    LARGE_INTEGER SystemExpirationDate;                    /* 0x2c8 */
    ULONG SuiteMask;                                       /* 0x2d0 */
    BOOLEAN KdDebuggerEnabled;                             /* 0x2d4 */
    UCHAR NXSupportPolicy;                                 /* 0x2d5 */
    USHORT CyclesPerYield;                                 /* 0x2d6 */
    volatile ULONG ActiveConsoleId;                        /* 0x2d8 */
    volatile ULONG DismountCount;                          /* 0x2dc */
    ULONG ComPlusPackage;                                  /* 0x2e0 */
    ULONG LastSystemRITEventTickCount;                     /* 0x2e4 */
    ULONG NumberOfPhysicalPages;                           /* 0x2e8 */
    BOOLEAN SafeBootMode;                                  /* 0x2ec */
    UCHAR VirtualizationFlags;                             /* 0x2ed */
    union {
        ULONG SharedDataFlags;                             /* 0x2f0 */
        struct {
            ULONG DbgErrorPortPresent       : 1;
            ULONG DbgElevationEnabed        : 1;
            ULONG DbgVirtEnabled            : 1;
            ULONG DbgInstallerDetectEnabled : 1;
            ULONG DbgLkgEnabled             : 1;
            ULONG DbgDynProcessorEnabled    : 1;
            ULONG DbgConsoleBrokerEnabled   : 1;
            ULONG DbgSecureBootEnabled      : 1;
            ULONG DbgMultiSessionSku        : 1;
            ULONG DbgMultiUsersInSessionSku : 1;
            ULONG DbgStateSeparationEnabled : 1;
            ULONG SpareBits                 : 21;
        } DUMMYSTRUCTNAME2;
    } DUMMYUNIONNAME2;
    ULONG DataFlagsPad[1];                                 /* 0x2f4 */
    ULONGLONG TestRetInstruction;                          /* 0x2f8 */
    LONGLONG QpcFrequency;                                 /* 0x300 */
    ULONG SystemCall;                                      /* 0x308 */
    union {
        ULONG AllFlags;                                    /* 0x30c */
        struct {
            ULONG Win32Process            : 1;
            ULONG Sgx2Enclave             : 1;
            ULONG VbsBasicEnclave         : 1;
            ULONG SpareBits               : 29;
        } DUMMYSTRUCTNAME;
    } UserCetAvailableEnvironments;
    ULONGLONG SystemCallPad[2];                            /* 0x310 */
    union {
        volatile KSYSTEM_TIME TickCount;                   /* 0x320 */
        volatile ULONG64 TickCountQuad;
    } DUMMYUNIONNAME;
    ULONG Cookie;                                          /* 0x330 */
    ULONG CookiePad[1];                                    /* 0x334 */
    LONGLONG ConsoleSessionForegroundProcessId;            /* 0x338 */
    ULONGLONG TimeUpdateLock;                              /* 0x340 */
    ULONGLONG BaselineSystemTimeQpc;                       /* 0x348 */
    ULONGLONG BaselineInterruptTimeQpc;                    /* 0x350 */
    ULONGLONG QpcSystemTimeIncrement;                      /* 0x358 */
    ULONGLONG QpcInterruptTimeIncrement;                   /* 0x360 */
    UCHAR QpcSystemTimeIncrementShift;                     /* 0x368 */
    UCHAR QpcInterruptTimeIncrementShift;                  /* 0x369 */
    USHORT UnparkedProcessorCount;                         /* 0x36a */
    ULONG EnclaveFeatureMask[4];                           /* 0x36c */
    ULONG TelemetryCoverageRound;                          /* 0x37c */
    USHORT UserModeGlobalLogger[16];                       /* 0x380 */
    ULONG ImageFileExecutionOptions;                       /* 0x3a0 */
    ULONG LangGenerationCount;                             /* 0x3a4 */
    ULONG ActiveProcessorAffinity;                         /* 0x3a8 */
    volatile ULONGLONG InterruptTimeBias;                  /* 0x3b0 */
    volatile ULONGLONG QpcBias;                            /* 0x3b8 */
    ULONG ActiveProcessorCount;                            /* 0x3c0 */
    volatile UCHAR ActiveGroupCount;                       /* 0x3c4 */
    union {
        USHORT QpcData;                                    /* 0x3c6 */
        struct {
            UCHAR volatile QpcBypassEnabled;
            UCHAR QpcShift;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME3;
    LARGE_INTEGER TimeZoneBiasEffectiveStart;              /* 0x3c8 */
    LARGE_INTEGER TimeZoneBiasEffectiveEnd;                /* 0x3d0 */
    XSTATE_CONFIGURATION XState;                           /* 0x3d8 */
} KSHARED_USER_DATA, *PKSHARED_USER_DATA;

#define SHARED_GLOBAL_FLAGS_QPC_BYPASS_ENABLED 0x01
#define SHARED_GLOBAL_FLAGS_QPC_BYPASS_USE_HV_PAGE 0x02
#define SHARED_GLOBAL_FLAGS_QPC_BYPASS_DISABLE_32BIT 0x04
#define SHARED_GLOBAL_FLAGS_QPC_BYPASS_USE_MFENCE 0x10
#define SHARED_GLOBAL_FLAGS_QPC_BYPASS_USE_LFENCE 0x20
#define SHARED_GLOBAL_FLAGS_QPC_BYPASS_A73_ERRATA 0x40
#define SHARED_GLOBAL_FLAGS_QPC_BYPASS_USE_RDTSCP 0x80

#define MAXIMUM_VOLUME_LABEL_LENGTH       (32 * sizeof(WCHAR))

#ifndef DEVICE_TYPE
#define DEVICE_TYPE ULONG
#endif
#define IRP_MJ_MAXIMUM_FUNCTION           0x1b
#define IRP_MJ_CREATE                     0x00
#define IRP_MJ_CREATE_NAMED_PIPE          0x01
#define IRP_MJ_CLOSE                      0x02
#define IRP_MJ_READ                       0x03
#define IRP_MJ_WRITE                      0x04
#define IRP_MJ_QUERY_INFORMATION          0x05
#define IRP_MJ_SET_INFORMATION            0x06
#define IRP_MJ_QUERY_EA                   0x07
#define IRP_MJ_SET_EA                     0x08
#define IRP_MJ_FLUSH_BUFFERS              0x09
#define IRP_MJ_QUERY_VOLUME_INFORMATION   0x0a
#define IRP_MJ_SET_VOLUME_INFORMATION     0x0b
#define IRP_MJ_DIRECTORY_CONTROL          0x0c
#define IRP_MJ_FILE_SYSTEM_CONTROL        0x0d
#define IRP_MJ_DEVICE_CONTROL             0x0e
#define IRP_MJ_INTERNAL_DEVICE_CONTROL    0x0f
#define IRP_MJ_SHUTDOWN                   0x10
#define IRP_MJ_LOCK_CONTROL               0x11
#define IRP_MJ_CLEANUP                    0x12
#define IRP_MJ_CREATE_MAILSLOT            0x13
#define IRP_MJ_QUERY_SECURITY             0x14
#define IRP_MJ_SET_SECURITY               0x15
#define IRP_MJ_POWER                      0x16
#define IRP_MJ_SYSTEM_CONTROL             0x17
#define IRP_MJ_DEVICE_CHANGE              0x18
#define IRP_MJ_QUERY_QUOTA                0x19
#define IRP_MJ_SET_QUOTA                  0x1a
#define IRP_MJ_PNP                        0x1b

#define IRP_MN_START_DEVICE                 0x00
#define IRP_MN_QUERY_REMOVE_DEVICE          0x01
#define IRP_MN_REMOVE_DEVICE                0x02
#define IRP_MN_CANCEL_REMOVE_DEVICE         0x03
#define IRP_MN_STOP_DEVICE                  0x04
#define IRP_MN_QUERY_STOP_DEVICE            0x05
#define IRP_MN_CANCEL_STOP_DEVICE           0x06
#define IRP_MN_QUERY_DEVICE_RELATIONS       0x07
#define IRP_MN_QUERY_INTERFACE              0x08
#define IRP_MN_QUERY_CAPABILITIES           0x09
#define IRP_MN_QUERY_RESOURCES              0x0A
#define IRP_MN_QUERY_RESOURCE_REQUIREMENTS  0x0B
#define IRP_MN_QUERY_DEVICE_TEXT            0x0C
#define IRP_MN_FILTER_RESOURCE_REQUIREMENTS 0x0D
#define IRP_MN_READ_CONFIG                  0x0F
#define IRP_MN_WRITE_CONFIG                 0x10
#define IRP_MN_EJECT                        0x11
#define IRP_MN_SET_LOCK                     0x12
#define IRP_MN_QUERY_ID                     0x13
#define IRP_MN_QUERY_PNP_DEVICE_STATE       0x14
#define IRP_MN_QUERY_BUS_INFORMATION        0x15
#define IRP_MN_DEVICE_USAGE_NOTIFICATION    0x16
#define IRP_MN_SURPRISE_REMOVAL             0x17
#define IRP_MN_QUERY_LEGACY_BUS_INFORMATION 0x18

#define IRP_MN_WAIT_WAKE                    0x00
#define IRP_MN_POWER_SEQUENCE               0x01
#define IRP_MN_SET_POWER                    0x02
#define IRP_MN_QUERY_POWER                  0x03

#define IRP_QUOTA_CHARGED               0x01
#define IRP_ALLOCATED_MUST_SUCCEED      0x02
#define IRP_ALLOCATED_FIXED_SIZE        0x04
#define IRP_LOOKASIDE_ALLOCATION        0x08

typedef struct _VPB {
  CSHORT  Type;
  CSHORT  Size;
  USHORT  Flags;
  USHORT  VolumeLabelLength;
  struct _DEVICE_OBJECT  *DeviceObject;
  struct _DEVICE_OBJECT  *RealDevice;
  ULONG  SerialNumber;
  ULONG  ReferenceCount;
  WCHAR  VolumeLabel[MAXIMUM_VOLUME_LABEL_LENGTH / sizeof(WCHAR)];
} VPB, *PVPB;

struct _KDPC;
struct _KAPC;
struct _IRP;
struct _DEVICE_OBJECT;
struct _DRIVER_OBJECT;
struct _KPROCESS;

typedef ULONG_PTR KSPIN_LOCK, *PKSPIN_LOCK;

typedef VOID (WINAPI *PKDEFERRED_ROUTINE)(struct _KDPC *, PVOID, PVOID, PVOID);
typedef VOID (WINAPI *PKSTART_ROUTINE)(PVOID);
typedef VOID (WINAPI *PINTERFACE_REFERENCE)(PVOID  Context);
typedef VOID (WINAPI *PINTERFACE_DEREFERENCE)(PVOID Context);

typedef NTSTATUS (WINAPI *PDRIVER_INITIALIZE)(struct _DRIVER_OBJECT *, PUNICODE_STRING);
typedef NTSTATUS (WINAPI *PDRIVER_DISPATCH)(struct _DEVICE_OBJECT *, struct _IRP *);
typedef void (WINAPI *PDRIVER_STARTIO)(struct _DEVICE_OBJECT *, struct _IRP *);
typedef void (WINAPI *PDRIVER_UNLOAD)(struct _DRIVER_OBJECT *);
typedef NTSTATUS (WINAPI *PDRIVER_ADD_DEVICE)(struct _DRIVER_OBJECT *, struct _DEVICE_OBJECT *);
typedef void (WINAPI *PDEVICE_CHANGE_COMPLETE_CALLBACK)(void*);

typedef struct _DISPATCHER_HEADER {
  UCHAR  Type;
  UCHAR  Absolute;
  UCHAR  Size;
  UCHAR  Inserted;
  LONG  SignalState;
  LIST_ENTRY  WaitListHead;
} DISPATCHER_HEADER, *PDISPATCHER_HEADER;

typedef struct _KEVENT {
  DISPATCHER_HEADER  Header;
} KEVENT, *PKEVENT, *RESTRICTED_POINTER PRKEVENT;

typedef struct _IO_TIMER *PIO_TIMER;
typedef struct _IO_TIMER_ROUTINE *PIO_TIMER_ROUTINE;
typedef struct _ETHREAD *PETHREAD;
typedef struct _KTHREAD *PKTHREAD, *PRKTHREAD;
typedef struct _EPROCESS *PEPROCESS;
typedef struct _KPROCESS KPROCESS, *PKPROCESS, *PRKPROCESS;
typedef struct _IO_WORKITEM *PIO_WORKITEM;
typedef struct _OBJECT_TYPE *POBJECT_TYPE;
typedef struct _OBJECT_HANDLE_INFORMATION *POBJECT_HANDLE_INFORMATION;
typedef struct _ZONE_HEADER *PZONE_HEADER;
typedef struct _LOOKASIDE_LIST_EX *PLOOKASIDE_LIST_EX;

typedef struct _KDPC {
  union {
    ULONG TargetInfoAsUlong;
    struct {
      UCHAR  Type;
      UCHAR  Importance;
      volatile USHORT  Number;
    } DUMMYSTRUCTNAME;
  } DUMMYUNIONNAME;
  SINGLE_LIST_ENTRY  DpcListEntry;
  KAFFINITY  ProcessorHistory;
  PKDEFERRED_ROUTINE  DeferredRoutine;
  PVOID  DeferredContext;
  PVOID  SystemArgument1;
  PVOID  SystemArgument2;
  PVOID  DpcData;
} KDPC, *PKDPC, *RESTRICTED_POINTER PRKDPC;

typedef enum _KDPC_IMPORTANCE {
  LowImportance,
  MediumImportance,
  HighImportance,
  MediumHighImportance
} KDPC_IMPORTANCE;

typedef struct _KDEVICE_QUEUE_ENTRY {
  LIST_ENTRY  DeviceListEntry;
  ULONG  SortKey;
  BOOLEAN  Inserted;
} KDEVICE_QUEUE_ENTRY, *PKDEVICE_QUEUE_ENTRY,
*RESTRICTED_POINTER PRKDEVICE_QUEUE_ENTRY;

typedef struct _WAIT_CONTEXT_BLOCK {
  KDEVICE_QUEUE_ENTRY  WaitQueueEntry;
  struct _DRIVER_CONTROL  *DeviceRoutine;
  PVOID  DeviceContext;
  ULONG  NumberOfMapRegisters;
  PVOID  DeviceObject;
  PVOID  CurrentIrp;
  PKDPC  BufferChainingDpc;
} WAIT_CONTEXT_BLOCK, *PWAIT_CONTEXT_BLOCK;

typedef struct _KDEVICE_QUEUE {
  CSHORT  Type;
  CSHORT  Size;
  LIST_ENTRY  DeviceListHead;
  KSPIN_LOCK  Lock;
  BOOLEAN  Busy;
} KDEVICE_QUEUE, *PKDEVICE_QUEUE, *RESTRICTED_POINTER PRKDEVICE_QUEUE;

typedef struct _DEVICE_OBJECT {
  CSHORT  Type;
  USHORT  Size;
  LONG  ReferenceCount;
  struct _DRIVER_OBJECT  *DriverObject;
  struct _DEVICE_OBJECT  *NextDevice;
  struct _DEVICE_OBJECT  *AttachedDevice;
  struct _IRP  *CurrentIrp;
  PIO_TIMER  Timer;
  ULONG  Flags;
  ULONG  Characteristics;
  PVPB  Vpb;
  PVOID  DeviceExtension;
  DEVICE_TYPE  DeviceType;
  CCHAR  StackSize;
  union {
    LIST_ENTRY  ListEntry;
    WAIT_CONTEXT_BLOCK  Wcb;
  } Queue;
  ULONG  AlignmentRequirement;
  KDEVICE_QUEUE  DeviceQueue;
  KDPC  Dpc;
  ULONG  ActiveThreadCount;
  PSECURITY_DESCRIPTOR  SecurityDescriptor;
  KEVENT  DeviceLock;
  USHORT  SectorSize;
  USHORT  Spare1;
  struct _DEVOBJ_EXTENSION  *DeviceObjectExtension;
  PVOID  Reserved;
} DEVICE_OBJECT;
typedef struct _DEVICE_OBJECT *PDEVICE_OBJECT;

typedef struct _DEVICE_RELATIONS {
  ULONG Count;
  PDEVICE_OBJECT Objects[1];
} DEVICE_RELATIONS;
typedef struct _DEVICE_RELATIONS *PDEVICE_RELATIONS;

typedef struct _DRIVER_EXTENSION {
  struct _DRIVER_OBJECT  *DriverObject;
  PDRIVER_ADD_DEVICE AddDevice;
  ULONG  Count;
  UNICODE_STRING  ServiceKeyName;
} DRIVER_EXTENSION, *PDRIVER_EXTENSION;

typedef struct _DRIVER_OBJECT {
  CSHORT  Type;
  CSHORT  Size;
  PDEVICE_OBJECT  DeviceObject;
  ULONG  Flags;
  PVOID  DriverStart;
  ULONG  DriverSize;
  PVOID  DriverSection;
  PDRIVER_EXTENSION  DriverExtension;
  UNICODE_STRING  DriverName;
  PUNICODE_STRING  HardwareDatabase;
  PVOID  FastIoDispatch;
  PDRIVER_INITIALIZE DriverInit;
  PDRIVER_STARTIO    DriverStartIo;
  PDRIVER_UNLOAD     DriverUnload;
  PDRIVER_DISPATCH   MajorFunction[IRP_MJ_MAXIMUM_FUNCTION + 1];
} DRIVER_OBJECT;
typedef struct _DRIVER_OBJECT *PDRIVER_OBJECT;
