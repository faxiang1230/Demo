infinity = float("inf")
graph = {}
graph["start"]={}
graph["start"]["A"]=5
graph["start"]["B"]=2
graph["A"]={}
graph["A"]["C"]=4
graph["A"]["D"]=2
graph["B"]={}
graph["B"]["A"]=8
graph["B"]["D"]=7
graph["C"]={}
graph["C"]["D"]=6
graph["C"]["final"]=3
graph["D"]={}
graph["D"]["final"]=1
graph["final"]={}

costs={}
costs["A"]=5
costs["B"]=2
costs["C"]=infinity
costs["D"]=infinity
costs["final"]=infinity

parents={}
parents["A"]="start"
parents["B"]="start"
parents["C"]=None
parents["D"]=None
parents["final"]=None

processed=[]
def find_lowest_cost_node(costs):
    low_cost = infinity
    low_key = None
    if costs is not None:
        for key in costs:
            if key not in processed and costs[key] < low_cost:
                low_cost = costs[key]
                low_key = key
    return low_key

node = find_lowest_cost_node(costs)
while node is not None:
    cost = costs[node]
    neighbors = graph[node]
    for n in neighbors.keys():
        new_cost = cost + neighbors[n]
        if costs[n] > new_cost:
            costs[n] = new_cost
            parents[n] = node
    processed.append(node)
    node = find_lowest_cost_node(costs)

print costs