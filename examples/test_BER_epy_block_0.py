import numpy as np
from gnuradio import gr
import random

class blk(gr.sync_block):
    """Generate reference and received bits with variable error rate"""

    def __init__(self, error_rate=0.10, pattern_len=50000):
        gr.sync_block.__init__(
            self,
            name='BER Pattern Generator',
            in_sig=[],
            out_sig=[np.uint8, np.uint8]  # ref and rx outputs
        )
        self.error_rate = error_rate
        self.prev_error_rate = error_rate
        self.pattern_len = pattern_len
        self.index = 0
        
        self.need_regen = True
        # Generate initial pattern
        self.regenerate_pattern()
    
    def regenerate_pattern(self):
        """Generate new reference and received patterns"""
        random.seed()  # Use current time as seed
        
        # Generate random reference bits
        self.ref_bits = [random.randint(0, 1) for _ in range(self.pattern_len)]
        
        # Generate received bits with errors
        self.rx_bits = []
        for bit in self.ref_bits:
            if random.random() < self.error_rate:
                # Inject error
                self.rx_bits.append(1 - bit)
            else:
                # No error
                self.rx_bits.append(bit)
        
        self.index = 0
        self.need_regen = False
        print(f"Generated {self.pattern_len} bits with {self.error_rate*100:.3f}% error rate")

    def work(self, input_items, output_items):
        # Check if we need to regenerate before producing output

        if (self.error_rate != self.prev_error_rate):
            self.need_regen = True
            self.prev_error_rate = self.error_rate
        
        if self.need_regen:
            print("Regenerating pattern in work()...")
            self.regenerate_pattern()
        
        out_ref = output_items[0]
        out_rx = output_items[1]
        
        for i in range(len(out_ref)):
            out_ref[i] = self.ref_bits[self.index]
            out_rx[i] = self.rx_bits[self.index]
            self.index = (self.index + 1) % self.pattern_len
        
        return len(out_ref)
