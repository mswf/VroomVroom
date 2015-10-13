#ifndef Clock_h
#define Clock_h

#include "typedef.h"

class Clock
{
        uint64 timeCycles;
        float timeScale;
        bool isPaused;
    
        static float cyclesPerSecond;
    
        static inline uint64 SecondsToCycles( float timeSeconds );
    
        // WARNING: Dangerous -- only use to convert small durations into seconds
        static inline float CyclesToSeconds( uint64 timeCycles );
    
    public:
    
        // Construct a clock. 'Explicit' is used to prevent
        // automatic conversion from float to Clock.
        explicit Clock( float startTimeSeconds = 0.0f );
    
        // Call this when game first starts up.
        static void Init();
    
        // This function should be called once per frame,
        // with the real measured frame time delta in seconds
        void Update( float deltaRealSeconds );

    
        float CalculateDeltaSeconds( const Clock& other );

        uint64 GetTimeCycles() const;
        float GetTimeScale() const;
        void SetPause( bool pause );
        void SetTimeScale( float scale );
        void SingleStep();
        bool IsPaused() const;
    
};

#endif /* Clock_h */
