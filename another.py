import hashlib
import bisect
from collections import defaultdict


class ConsistentHashing:
    def __init__(self, replication_factor=3, virtual_nodes=1):
        self.replication_factor = replication_factor
        self.virtual_nodes = virtual_nodes
        self.hash_ring = []
        self.server_map = {}
        self.data_map = defaultdict(list)  # Server-wise data storage
        self.key_table = defaultdict(list)  # Table for each server handling keys

    def _hash(self, key):
        """Generates a hash for the given key."""
        return int(hashlib.sha256(key.encode('utf-8')).hexdigest(), 16)

    def _add_virtual_nodes(self, server):
        """Adds virtual nodes for a given server."""
        for v_node in range(self.virtual_nodes):
            virtual_key = f"{server}-v{v_node}"
            v_hash = self._hash(virtual_key)
            bisect.insort(self.hash_ring, v_hash)
            self.server_map[v_hash] = server

    def add_server(self, server):
        """Adds a new server to the hash ring."""
        self._add_virtual_nodes(server)

    def remove_server(self, server):
        """Removes a server and its virtual nodes from the hash ring."""
        for v_node in range(self.virtual_nodes):
            virtual_key = f"{server}-v{v_node}"
            v_hash = self._hash(virtual_key)
            self.hash_ring.remove(v_hash)
            del self.server_map[v_hash]
        # Reassign data to other servers
        keys_to_reassign = self.key_table[server]
        del self.key_table[server]
        for key in keys_to_reassign:
            value = next((v for k, v in self.data_map[server] if k == key), None)
            if value:
                self.store_data(key, value)
        del self.data_map[server]

    def get_server(self, key):
        """Finds the server responsible for a given key."""
        key_hash = self._hash(key)
        pos = bisect.bisect_right(self.hash_ring, key_hash)
        if pos == len(self.hash_ring):
            pos = 0
        return self.server_map[self.hash_ring[pos]]

    def replicate_data(self, key):
        """Returns a list of servers where replicas of the data should be stored."""
        key_hash = self._hash(key)
        pos = bisect.bisect_right(self.hash_ring, key_hash)
        servers = []
        for _ in range(self.replication_factor):
            if pos == len(self.hash_ring):
                pos = 0
            server = self.server_map[self.hash_ring[pos]]
            if server not in servers:  # Ensure unique servers
                servers.append(server)
            pos += 1
        return servers

    def store_data(self, key, value):
        """Stores data on the primary server and its replicas."""
        primary_server = self.get_server(key)
        replicas = self.replicate_data(key)
        for server in replicas:
            self.data_map[server].append((key, value))
            self.key_table[server].append(key)  # Update key table
        return primary_server, replicas

    def remove_data(self, key):
        """Removes data associated with a key from all servers."""
        replicas = self.replicate_data(key)
        for server in replicas:
            self.data_map[server] = [(k, v) for k, v in self.data_map[server] if k != key]
            if key in self.key_table[server]:
                self.key_table[server].remove(key)

    def print_state(self):
        """Prints the current state of the hash ring, data distribution, and key tables."""
        print("Hash Ring:", self.hash_ring)
        print("Server Map:", self.server_map)
        print("Data Map:")
        for server, data in self.data_map.items():
            print(f"  {server}: {data}")
        print("Key Table:")
        for server, keys in self.key_table.items():
            print(f"  {server}: {keys}")


# Example Usage
if __name__ == "__main__":
    # Initialize consistent hashing with replication factor = 3, and virtual nodes = 2
    ch = ConsistentHashing(replication_factor=3, virtual_nodes=2)

    # Add servers
    for server in ["s0", "s1", "s2", "s3"]:
        ch.add_server(server)

    # Store data
    print("Storing data...")
    for i in range(5):
        key = f"key{i}"
        primary, replicas = ch.store_data(key, f"value{i}")
        print(f"{key} stored on primary: {primary}, replicas: {replicas}")

    # Print hash ring, data map, and key table state
    ch.print_state()

    # Remove a server
    print("\nRemoving server 's1'...")
    ch.remove_server("s1")
    ch.print_state()