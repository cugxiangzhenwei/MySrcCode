
#ifndef __MD5_H__
#define __MD5_H__

#include <string>

/// A class to compute and check MD5 digests
/// @short A standard MD5 digest calculator
//## class MD5 
class MD5
{
    ////    Constructors and destructors    ////
public :

    /// 
    /// Construct a fresh initialized instance
    //## operation MD5() 
    MD5();

    /// 
    ///  Copy constructor
    ///  @param original MD5 instance to copy
    //## operation MD5(const MD5 &) 
    MD5(const MD5& original);

    /// 
    ///  Construct a digest from a buffer of data
    ///  @param buf Pointer to the data to be included in digest
    ///  @param len Length of data in the buffer
    //## operation MD5(const void *, int) 
    MD5(const void* buf, int len);

    ///  Construct a digest from a String
    ///  @param str String to be included in digest
    //## operation MD5(const OSString &) 
    MD5(const std::string& str);

    /// 
    ///  Destroy the instance, free allocated memory
    //## operation ~MD5() 
    ~MD5();


    ////    Operations    ////
public :

    ///
    /// Clear the digest and prepare for reuse
    //## operation Clear() 
    void Clear();

    ///
    /// Finalize the digest computation, make result ready.
    /// Subsequent calls to @ref Update() will fail
    //## operation Finalize() 
    void Finalize();

    ///
    /// Returns the standard hexadecimal representation of the message digest.
    /// The digest is finalized if if wasn't already
    /// @return A String which holds the hex digest or a null one if some error occured
    //## operation HexDigest() 
    const std::string& HexDigest();

    ///
    /// Returns a pointer to the raw 16-byte binary value of the message digest.
    /// The digest is finalized if if wasn't already
    /// @return Pointer to the raw digest data or NULL if some error occured
    //## operation RawDigest() 
    const unsigned char* RawDigest();

    ///
    /// Update the digest from a buffer of data
    /// @param buf Pointer to the data to be included in digest
    /// @param len Length of data in the buffer
    /// @return True if success, false if @ref Finalize() was already called
    //## operation Update(const void *, unsigned int) 
    bool Update(const void* buf, unsigned int len);

    ///
    /// Update the digest from the content of a String
    /// @param str String to be included in digest
    /// @return True if success, false if @ref Finalize() was already called
    //## operation Update(const OSString &) 
    inline bool Update(const std::string& str)
    {
        //#[ operation Update(const OSString &) 
        return Update(str.c_str(), str.length());
        //#]
    }


    ///
    /// MD5 updating operator for Strings
    //## operation operator<<(const OSString &) 
    inline MD5& operator<<(const std::string& value)
    {
        //#[ operation operator<<(const OSString &) 
        Update(value); return *this;
        //#]
    }


    ///
    /// MD5 updating operator for C strings
    //## operation operator<<(const char *) 
    MD5& operator << (const char* value);

    ///
    /// Assignment operator.
    //## operation operator=(const MD5 &) 
    MD5& operator=(const MD5& original);

private :

    //## operation Init() 
    void Init();


    ////    Attributes    ////
private :

    unsigned char m_bin[16];        //## attribute m_bin 

    void* m_private;        //## attribute m_private 


    ////    Relations and components    ////
private :

    std::string m_hex;     //## classInstance m_hex 
};

#endif



