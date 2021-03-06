// libaff4_devio.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "libaff4_devio.h"

using namespace std;
using namespace aff4;
using namespace aff4::container;
using namespace aff4::rdf;

struct Session
{
    shared_ptr<IAFF4Container> container;
    shared_ptr<IAFF4Image> image;
    shared_ptr<IAFF4Map> map;
    shared_ptr<IAFF4Stream> stream;
};

// This is an example of an exported function.
void *dllopen(const char *file,
    int read_only,
    dllread_proc *pdllread,
    dllwrite_proc *pdllwrite,
    dllclose_proc *pdllclose,
    off_t_64 *size)
{
    if (!read_only)
    {
        cout << "AFF4 images can only be opened in read-only mode." << endl;
        errno = EACCES;
        return NULL;
    }

    cout << "Image path '" << file << "'..." << endl;

    auto index_str = strstr(file, MULTI_CONTAINER_DELIMITER);

    if (index_str == nullptr)
    {
        cout << "Missing index paramter. Syntax: imagefile" MULTI_CONTAINER_DELIMITER "index" << endl;
        errno = EINVAL;
        return NULL;
    }

    string filename(file, index_str);

    index_str += _countof(MULTI_CONTAINER_DELIMITER) - 1;

    char *ptr;
    auto index = strtoul(index_str, &ptr, 0);

    if (ptr == index_str)
    {
        cout << "Invalid index number: '" << index_str << "'" << endl;
        errno = EINVAL;
        return NULL;
    }

    cout << "Opening image file '" << filename << "' index " << index << "..." << endl;

    unique_ptr<IAFF4Resolver> resolver(createResolver(filename));
    if (resolver == nullptr)
    {
        cout << "Unable to construct a resolver for file \"" << filename << "\"." << endl;
        return NULL;
    }

    shared_ptr<IAFF4Container> container(openAFF4Container(filename, resolver.get()));

    if (container == nullptr)
    {
        cout << "Unable to open the file \"" << filename << "\"." << endl;
        return NULL;
    }

    /*
     * Print the container's resource values.
     */

    cout << "Container: " << container->getResourceID() << endl;

    /*
     * Get a vector of all aff4:Image instances in this container
     */

    auto images = container->getImages();

    auto num_images = images.size();

    cout << "Number of images in container: " << num_images << endl;

    if (index >= num_images)
    {
        cout << "Image index " << index << " not found." << endl;
        return NULL;
    }

    auto image = images[index];

    /*
     * Print the image's resource values.
     */

    cout << "Image " << index << ": " << image->getResourceID() << endl;

    /*
     * Get the map instance of the image, and create a digest of the primary stream.
     */

    auto map = image->getMap();
    if (map != nullptr)
    {
        auto stream = map->getStream();
        if (stream != nullptr)
        {
            *size = stream->size();

            printf("Image virtual size is: %I64i bytes\n", *size);

            *pdllread = ::dllread;
            *pdllwrite = NULL;
            *pdllclose = ::dllclose;
            
            return new Session{ container, image, map, stream };
        }

        cout << "Cannot open image as stream." << endl;
        return NULL;
    }

    cout << "Cannot open image mapping object." << endl;
    return NULL;
}

safeio_ssize_t dllread(void *fd,
    void *buf,
    safeio_size_t size,
    off_t_64 offset)
{
    if (size == 0)
        return 0;

    auto session = (Session*)fd;

#if _DEBUG
    cout << "Reading from obj " << fd << " offset " << offset << ", requested " << size << " bytes." << endl;
#endif

    auto rc = session->stream->read(buf, size, offset);

    if (rc < 0)
    {
        cout << "Read error at " << offset << ", requested " << size << " bytes." << endl;
    }
    else if (rc != size)
    {
        cout << "Incomplete read at " << offset << ", requested " << size << " bytes, got " << rc << "." << endl;
    }

    return (safeio_ssize_t)rc;
}

int dllclose(void *fd)
{
    auto session = (Session*)fd;

    if (session != NULL)
    {
        session->stream->close();
        session->container->close();
        delete session;
    }

    return 1;
}

uint32_t getsectorsize(void *fd)
{
    if (fd == NULL)
    {
        errno = EBADF;
        return 0;
    }

    auto session = (Session*)fd;

    // Get Sector Size.
    auto properties = session->image->getProperty(Lexicon::AFF4_BLOCKSIZE);
    if (!properties.empty())
    {
        RDFValue v = properties[0];
        if (v.getXSDType() == XSDType::Int)
        {
            return (uint32_t)v.getInteger();
        }
        else if (v.getXSDType() == XSDType::Long)
        {
            return (uint32_t)v.getLong();
        }
    }

    errno = EINVAL;
    return 0;
}

uint32_t getimagecount(const char *file)
{
    string filename(file);

    cout << "Opening image container '" << filename << "'..." << endl;

    unique_ptr<IAFF4Resolver> resolver(createResolver(filename));
    if (resolver == nullptr)
    {
        cout << "Unable to construct a resolver for file \"" << filename << "\"." << endl;
        return NULL;
    }

    shared_ptr<IAFF4Container> container(openAFF4Container(filename, resolver.get()));

    if (container == nullptr)
    {
        cout << "Unable to open the file \"" << filename << "\"." << endl;
        return NULL;
    }

    /*
     * Print the container's resource values.
     */

    cout << "Container: " << container->getResourceID() << endl;

    /*
     * Get a vector of all aff4:Image instances in this container
     */

    auto images = container->getImages();

    auto num_images = images.size();

    cout << "Number of images in container: " << num_images << endl;

    return (uint32_t)num_images;
}

int getlasterrorcode()
{
    return errno;
}

const char *geterrormessage(int errorcode)
{
    return strerror(errorcode);
}
