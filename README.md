# Metrolink
Home assignment for Metrolink.

Sums transaction JSON logs

### Behaviour

Groupbys arbitrary number of columns based on config file and sums the column 'moneySpent'.

Malformed JSONs are skipped.

Writes results to '\<Input Folder\>\results.json'

### Usage 
MetroLink-JsonAggtor.exe \<Input Folder\> \<Config file\>
  
### External code
RapidJSON project
