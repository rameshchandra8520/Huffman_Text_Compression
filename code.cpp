#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

// A Tree node
struct Node
{
	char ch;
	int freq;
	Node *left, *right;
};

// Function to allocate a new tree node
Node* getNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

// Comparison object to be used to order the heap
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		// highest priority item has lowest frequency
		return l->freq > r->freq;
	}
};

// Encoding function to Traverse the Huffman Tree to generate Huffman Codes.
void encode(Node* root, string binStr, unordered_map<char, string> &huffmanCode)
{
	if (root == nullptr)
		return;

	// Found a leaf node (No left child & No right child)
	if (!root->left && !root->right) {
		// Store the Huffman Code for this character in the map
		huffmanCode[root->ch] = binStr;
        return;
	}
    
	// Traverse left child with '0' appended to the binary string
	encode(root->left, binStr + "0", huffmanCode);
	// Traverse right child with '1' appended to the binary string
	encode(root->right, binStr + "1", huffmanCode);
}

// traverse the Huffman Tree and decode the encoded string
void decode(Node* root, int &index, string str,string &decodedString)
{
	if (root == nullptr) {
		return;
	}

	// found a leaf node
	if (!root->left && !root->right)
	{
		cout << root->ch;
		decodedString+=root->ch;
		return;
	}

	index++;

	if (str[index] =='0')
		decode(root->left, index, str,decodedString);
	else
		decode(root->right, index, str,decodedString);
}

double calculateCompressionFactor(int inputSizeBits, int encodedSizeBits) {
    double compressionFactor = 1.0 - ((double)(encodedSizeBits) / inputSizeBits);
    return compressionFactor * 100.0; // Convertion to percentage % .
}

int hammingDistance(const string& str1, const string& str2) {
    int distance = 0;
    int len = min(str1.length(), str2.length());

    for (int i = 0; i < len; ++i) {
        if (str1[i] != str2[i]) {
            ++distance;
        }
    }

    // Add the remaining characters in the longer string, if any
    distance += abs((int)(str1.length() - str2.length()));

    return distance;
}

void HuffmanCompression(string text)
{
    // Count frequency of each character and store them in a map
	unordered_map<char, int> freq;
	for (char ch: text) {
		freq[ch]++;
	}

	// Create a priority queue to store the nodes of Huffman tree;
	priority_queue<Node*, vector<Node*>, comp> pq;

	// Create a leaf node for each character and add it to the priority queue.
	for (auto pair: freq) {
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	while (pq.size() != 1)
	{
		// Extract the two lowest frequency nodes as left and right children
        Node *left = pq.top(); pq.pop();
		Node *right = pq.top();	pq.pop();

        // Create an internal node combining the two lowest-frequency nodes
        // and enqueue it with a frequency equal to the sum of the children's frequencies.

		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	//Last node left will be root, root stores pointer to root of Huffman Tree.
	Node* root = pq.top();

	// Traverse the Huffman Tree to generate Huffman Codes and print them.
	unordered_map<char, string> huffmanCodes;
	encode(root, "", huffmanCodes);

	cout << "Huffman Codes are :\n" << '\n';
	for (auto pair: huffmanCodes) {
        // Char  || Huffman Codes
		cout << pair.first << " " << pair.second << '\n';
	}

	cout << "\nOriginal string was :\n" << text << '\n';

	// print encoded string
	string binStr = "";
	for (char ch: text) {
		binStr += huffmanCodes[ch];
	}

	cout << "\nEncoded string is :\n" << binStr << '\n';
    cout<<endl;

    // Calculating the size of input string in bits
    int inputSizeInBits = text.length()*8;
    cout<< "Size of input string in bits: " << inputSizeInBits << " bits"<<endl;

    // Calculating the encoded binary string size in bits
    int encodedSizeInBits = binStr.length();
    cout << "Encoded string size in bits: " << encodedSizeInBits << " bits" << endl;

    double compressionFactor = calculateCompressionFactor(inputSizeInBits,encodedSizeInBits);
    cout<< "Compression Percentage: " << compressionFactor << " %"<<endl;


	// Traverse the Huffman tree to decode the encoded string to get the orginal string.
	int index = -1;
	cout << "\nDecoded string is: \n";
	string decodedString;
	while (index < (int)binStr.size() - 2) {
		decode(root, index, binStr,decodedString);
	}

    // Using hamming distance, lets find the difference between the original string and the decoded string.
    // To check whether there is any loss in compression or not.

    int lossOfCompression = hammingDistance(text, decodedString);
	cout<<endl<<endl;
    cout <<"Lost bits due to Compression (Hamming Distance): " << lossOfCompression << " bits" << endl;

	if(!lossOfCompression){
		cout << "Which means it's a lossless Compression.";
	}

}




// Huffman coding algorithm
int main()
{
	string text;
    getline(cin, text);

	HuffmanCompression(text);

	return 0;
}