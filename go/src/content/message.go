package content

import (
	
)
const CONTENTPARSER_MAGIC = 0xFF

type Message struct {
	//cp_send_msg_head_t
	magic uint8 
	msg_type int32
	size uint32
	/*
	//cp_send_policy_filemap_t
	taskpath [128]uint8
    filepath [128]uint8
    filepath2 [128]uint8
    devicetype [16]uint8
    devicetype2 [16]uint8
    guid [64]uint8
    action [8]uint8
    pid	int32
    uid int32
    msgtype int32
    */
}

type Policy struct {
	rulename [128]byte
	keywords [128]byte
	filePath [128]byte
}
