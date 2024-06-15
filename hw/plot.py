nflops_per_cycle = 4
nprocessors = 1
max_GHz_of_processor = 5.2
max_gflops = nflops_per_cycle * nprocessors * max_GHz_of_processor

# L1 Cache: 1408 Kb ; L2 Cache: 14 MB ; L3 Cache: 30 MB
# 16-Core (8P+8E)
# 24 Threads
# Intel Turbo Boost Max Technology 3.0 Frequency: Up to 5.2 GHz
# Single P-core Turbo Frequency: Up to 5.1 GHz
# Single E-core Turbo Frequency: Up to 3.9 GHz
# 819.2 GFLOPS (https://www.intel.com/content/dam/support/us/en/documents/processors/APP-for-Intel-Core-Processors.pdf)
"""
https://en.wikichip.org/wiki/intel/microarchitectures/alder_lake
L1I Cache	32 KB/core
L1D Cache	48 KB (P) / 64 KB (E)/core
L1 Cache	80 KB (P) / 96 KB (E)/core
L2 Cache	1.25 MB (P) / 2MB (4E)/core
L3 Cache	up to 30 MB
"""
