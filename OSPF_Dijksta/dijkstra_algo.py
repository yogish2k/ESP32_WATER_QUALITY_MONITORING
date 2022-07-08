#Group 24 - Real time water quality monitoring using Wireless Sensor Networks.

#Dijkstra Algo
from math import *
from operator import itemgetter
import sys
import csv
import json

#=====================================================
# Reading json file for input
#=====================================================

def read_json(json_file_path):
    with open(json_file_path) as json_file:
        data = json.load(json_file)
    return data

#=====================================================
# Set initial weights. Infinity for non source nodes.
#=====================================================

def set_initial_weights(number_of_nodes, initial_node):
    weights = dict()
    for num in range(1, number_of_nodes+1):
        weights[num] = inf
    weights[initial_node] = 0
    return weights

#=====================================================
# Get Node info based on input.
#=====================================================

def get_node_info(nodes):
    node_info = dict()
    for node in range(len(nodes)):
        temp = nodes[node].split(",")
        if len(temp) != 3:
            print(f"{nodes[node]} is invalid")
            print("Give only initial node, target node and path weight as node information. Exiting...!")
            sys.exit(1)
        temp_dict = dict()
        temp_dict[int(temp[1])] = int(temp[2])
        if int(temp[0]) not in node_info:
            node_info[int(temp[0])] = [temp_dict]
        else:
            node_info[int(temp[0])].append(temp_dict)
    return node_info

#=====================================================
# Find closest neighbour
#=====================================================

def find_closest_neighbour(node_info, visited_nodes):
    weights = []
    for nodes in node_info:
        for node, weight in nodes.items():
            weights.append([node, weight])
    least_weight = sorted(weights, key = itemgetter(1))
    num = 0
    while(1):
        if least_weight[num][0] not in visited_nodes:
            break
        else:
            num += 1
    return least_weight[num][0]

#=====================================================
# Update weights after each iteration of Dijkstra.
#=====================================================

def update_weights(node_info, weights, present_node, visited_nodes, path):
    present_weight = weights[present_node]
    modified_weights = weights.copy()
    for nodes in node_info:
        for node, value in nodes.items():
            node_weight = int(value)
            if node in visited_nodes:
                continue
            elif weights[int(node)] > present_weight + node_weight:
                modified_weights[int(node)] = present_weight + node_weight
                path.append(node)
            else:
                continue
    return modified_weights

#=====================================================
# Find minimum weighted node among unvisited nodes.
#=====================================================

def get_min_weight_of_univisited_nodes(unvisited_nodes, weights):
    unvisited_nodes_weights = dict()
    for node in weights:
        if node in unvisited_nodes:
            unvisited_nodes_weights[node] = weights[node]
    
    unvisited_nodes_weights = sorted(unvisited_nodes_weights.items(), key = lambda x: x[1])
    return unvisited_nodes_weights[0][0]

#=====================================================
# Parent Dijkstra Function.
#=====================================================

def perform_dijkstra(nodes, source_node, node_count):
    initial_weights = set_initial_weights(node_count, source_node)
    visited_nodes = []
    unvisited_nodes = list(range(1, node_count+1))
    final_weights = initial_weights.copy()
    present_node = source_node
    present_node_list = []
    paths = dict()
    while(1):
        present_node_list.append(present_node)
        if len(unvisited_nodes) == 1:
            break
        visited_nodes.append(present_node)
        unvisited_nodes.remove(present_node)
        #closest_neighbour = find_closest_neighbour(nodes[present_node], visited_nodes)
        paths[present_node] = []
        final_weights = update_weights(nodes[present_node], final_weights, present_node, visited_nodes, paths[present_node])
        #print("===================")
        present_node = get_min_weight_of_univisited_nodes(unvisited_nodes, final_weights)
    return final_weights, paths

#=====================================================
# Find sink node among given nodes.
#=====================================================

def get_nodes_rms(dijkstra_nodes_info):
    rms_weights = dict()
    for nodes in dijkstra_nodes_info:
        temp = 0
        for node, value in dijkstra_nodes_info[nodes].items():
            temp += value
        rms_weights[nodes] = (temp**2)/len(dijkstra_nodes_info[nodes])
    return rms_weights

def write_to_csv(csv_file, config_object):
    file = open(csv_file, 'w', newline ='')
    nodes_output = config_object["nodes_data"]
    nodes = []
    src_dst = []
    final_values = []
    header = ["source", "destination", "weight"]
    for temp in nodes_output:
        values = temp.split(",")
        nodes.append(values)
        src_dst.append(values[:2])
    data = {tuple(sorted(item)) for item in src_dst}
    for node in nodes:
        if (node[0], node[1]) in data:
            final_values.append(node)
    print(sorted(final_values))

    # writing the data into the file
    with file:   
        write = csv.writer(file)
        write.writerow(header)
        write.writerows(sorted(final_values))


#=====================================================
# Main function.
#=====================================================

if __name__ == "__main__":
    json_file = ".practice\\programs\\nodes.json"
    data = read_json(json_file)     #print(data["nodes_data"])
    nodes = data["nodes_data"]
    node_count = data["nodes_count"]
    nodes = get_node_info(nodes)
    
    dijsktra_dict = dict()
    dijkstra_paths = dict()
    for node in range(1, node_count+1):
        temp = perform_dijkstra(nodes, node, node_count)
        dijsktra_dict[node] = temp[0]
        dijkstra_paths[node] = temp[1]

    report = "Dijkstra Algorithm report. \n\n"
    for node in dijsktra_dict:
        report+=(f"Final weights with node {node} as sink node: {dijsktra_dict[node]}\n")
    report += ("\n\n")
    report+=("Average weights of the network with various sink nodes - \n")
    for node, weight in get_nodes_rms(dijsktra_dict).items():
        report+=(f"{node}: {weight}\n")
    report += ("\n")
    optimal_node = min(get_nodes_rms(dijsktra_dict), key = get_nodes_rms(dijsktra_dict).get)
    report += f"Node best suited for sink node is {optimal_node}\n"
    report += f"Shortest paths : {dijkstra_paths[optimal_node]}"
    with open("report.txt", "w") as f:
        f.write(report)
    
    write_to_csv("nodes.csv", data)