import hashlib
from bisect import bisect_right

class HashChain:
    def __init__(self):
        self.servers = {}  # {hash: serverId}
        self.sorted_hashes = []  # Sorted list of server hashes
        self.key_map = {}  # Key-to-server mapping

    def hash_function(self, value):
        """Hashes a value into a 64-bit integer."""
        return int(hashlib.sha256(value.encode()).hexdigest(), 16) % (2 ** 64)

    def add_server(self, server_id):
        server_hash = self.hash_function(server_id)
        if server_hash in self.servers:
            print(f"Server {server_id} already exists on the chain.")
            return
        self.servers[server_hash] = server_id
        self.sorted_hashes.append(server_hash)
        self.sorted_hashes.sort()
        print(f"Added server {server_id} at hash {server_hash}")

        # Reassign keys if necessary
        self.reassign_keys()

    def remove_server(self, server_id):
        server_hash = self.hash_function(server_id)
        if server_hash not in self.servers:
            print(f"Server {server_id} does not exist.")
            return
        del self.servers[server_hash]
        self.sorted_hashes.remove(server_hash)
        print(f"Removed server {server_id} from hash {server_hash}")

        # Reassign keys if necessary
        self.reassign_keys()

    def add_key(self, key):
        key_hash = self.hash_function(key)
        server_hash = self.find_server(key_hash)
        self.key_map[key] = server_hash
        print(f"Key {key} assigned to server {self.servers[server_hash]}")

    def find_server(self, key_hash):
        """Finds the server responsible for a given key hash."""
        idx = bisect_right(self.sorted_hashes, key_hash)
        if idx == len(self.sorted_hashes):
            idx = 0  # Wrap around to the first server
        return self.sorted_hashes[idx]

    def reassign_keys(self):
        """Reassign keys to servers after server changes."""
        if not self.servers:
            self.key_map.clear()
            return
        for key, key_hash in [(key, self.hash_function(key)) for key in self.key_map]:
            server_hash = self.find_server(key_hash)
            self.key_map[key] = server_hash

    def display(self):
        print("Servers on the chain:")
        for server_hash in self.sorted_hashes:
            print(f"Server {self.servers[server_hash]} at hash {server_hash}")
        print("Key assignments:")
        for key, server_hash in self.key_map.items():
            print(f"Key {key} -> Server {self.servers[server_hash]}")

# Example usage
hash_chain = HashChain()
hash_chain.add_server("Server1")
hash_chain.add_server("Server2")
hash_chain.add_key("Key1")
hash_chain.add_key("Key2")
hash_chain.remove_server("Server1")
hash_chain.display()
