To make a python venv

```bash
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

To run the code
```bash
make -j
./ramulator configs/TLDRAM-config.cfg --mode=dram dram.trace
```

To turn on/off hrng, update the enable_hrng flag in TLDRAM-config.cfg

To run the test cases we presented on the report:
```bash
./run_test.sh
```
