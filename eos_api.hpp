// <ORACLIZE_API>
/*
Copyright (c) 2015-2016 Oraclize SRL
Copyright (c) 2016 Oraclize LTD
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <eosiolib/eosio.hpp>
#include <eosiolib/types.hpp>      
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h> 


using namespace eosio;


// UTILS                                            

std::string vector_to_string(std::vector<uint8_t> v){                                                    
    std::string v_str( v.begin(), v.end() );        
    return v_str;                                   
}                                                   

std::string checksum256_to_string(checksum256 c){   
    char hexstr[64];                                
    for (int i=0; i<32; i++) sprintf(hexstr+i*2, "%02x", c.hash[i]);                                     
    std::string c_str = std::string(hexstr);        
    return c_str;                                   
}                                                   

std::string chara_to_hexstring(uint8_t *input, int size) {                                               
    char hexstr[size*2];                            

    for (int i=0; i<size; i++) {                    
        sprintf(hexstr+i*2, "%02x", input[i]);      
    }                                               
    std::string c_str = std::string(hexstr);        

    return c_str;                                   
}                                                   

std::string vector_to_hexstring(std::vector<uint8_t> *input) {                                           
    int size = input->size();                       
    char hexstr[size*2];                            

    for (int i=0; i<size; i++) {                    
        sprintf(hexstr+i*2, "%02x", input->at(i));  
    }                                               
    std::string c_str = std::string(hexstr);        

    return c_str;                                   
}  


// API


#ifndef ORACLIZE_PAYER
  #define ORACLIZE_PAYER _self
#endif

#define oraclize_query(...) oraclize_query__(ORACLIZE_PAYER, __VA_ARGS__, _self)


// @abi table
struct snonce
{
    uint64_t sender;
    uint32_t nonce;
    
    uint64_t primary_key() const { return sender; }
    
};


// @abi table                      
struct cbaddr
{                                  
    account_name sender;               
    
    account_name primary_key() const { return sender; }
};

typedef eosio::multi_index<N(snonce), snonce> ds_snonce;
typedef eosio::multi_index<N(cbaddr), cbaddr> ds_cbaddr;


account_name oraclize_cbAddress(){
    ds_cbaddr cb_addrs(N(oraclizeconn), N(oraclizeconn));
    auto itr = cb_addrs.begin();
    account_name cbaddr = (itr != cb_addrs.end()) ? itr->sender : 0;
    return cbaddr;
}

uint32_t oraclize_getSenderNonce(account_name sender){
    ds_snonce last_nonces(N(oraclizeconn), N(oraclizeconn));
    auto itr = last_nonces.find(sender);
    uint32_t nonce = 0;
    if (itr != last_nonces.end()){
        nonce = itr->nonce;
    }
    return nonce;
}



checksum256 oraclize_getNextQueryId(account_name sender){
    uint32_t nonce = oraclize_getSenderNonce(sender);
    uint64_t curr_time = current_time();
    uint32_t now_ = now();
    size_t tx_size = transaction_size();
    int tapos_block_num_ = tapos_block_num();

    uint8_t tbh[sizeof(sender)+sizeof(nonce)+sizeof(curr_time)+sizeof(now_)+sizeof(tx_size)+sizeof(tapos_block_num_)]; //account_name[uint64_t] + nonce[uint32_t]  +  current_time() + now() + transaction_size + tapos_block_num
    std::memcpy(tbh, &sender, sizeof(sender));
    std::memcpy(tbh+sizeof(sender), &nonce, sizeof(nonce));

    std::memcpy(tbh+sizeof(sender)+sizeof(nonce), &curr_time, sizeof(curr_time));
    std::memcpy(tbh+sizeof(sender)+sizeof(nonce)+sizeof(curr_time), &now_, sizeof(now_));
    std::memcpy(tbh+sizeof(sender)+sizeof(nonce)+sizeof(curr_time)+sizeof(now_), &tx_size, sizeof(tx_size));
    std::memcpy(tbh+sizeof(sender)+sizeof(nonce)+sizeof(curr_time)+sizeof(now_)+sizeof(tx_size), &tapos_block_num_, sizeof(tapos_block_num_));
    
    checksum256 calc_hash;
    sha256((char *)tbh, sizeof(tbh), &calc_hash);
    
    return calc_hash;
}


checksum256 oraclize_query__(account_name user, unsigned int timestamp, std::string datasource, std::string query, uint8_t prooftype, account_name sender){
    checksum256 queryId = oraclize_getNextQueryId(sender);
    action(
        permission_level{ user, N(active) },
        N(oraclizeconn), N(query),
        std::make_tuple(sender, (int8_t)1, (uint32_t)timestamp, queryId, datasource, query, prooftype)
      ).send();
    return queryId;
}

checksum256 oraclize_query__(account_name user, std::string datasource, std::string query, account_name sender){
    return oraclize_query__(user, 0, datasource, query, 0, sender);
}                                  

checksum256 oraclize_query__(account_name user, unsigned int timestamp, std::string datasource, std::string query, account_name sender){    
    return oraclize_query__(user, timestamp, datasource, query, 0, sender);
}                                  

checksum256 oraclize_query__(account_name user, std::string datasource, std::string query, uint8_t prooftype, account_name sender){
    return oraclize_query__(user, 0, datasource, query, prooftype, sender);
}


// CONSTANTS

const uint8_t proofType_NONE = 0x00;
const uint8_t proofType_TLSNotary = 0x10;
const uint8_t proofType_Ledger = 0x30;
const uint8_t proofType_Android = 0x40;
const uint8_t proofType_Native = 0xF0;
const uint8_t proofStorage_IPFS = 0x01;
