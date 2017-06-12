#include "opencv/highgui.h"
#include "../../src/FrameHelper.h"

#include <rtt/typelib/TypelibMarshallerBase.hpp>
#include <rice/Constructor.hpp>
#include <rice/Object.hpp>
#include <rice/Array.hpp>

#include <typelib/value.hh>
#include <typelib_ruby.hh>

#include <iostream>

using namespace Rice;
using namespace frame_helper;
using namespace base::samples::frame;

//TODO this could be generated automatically by typelib
template<> Frame* from_ruby<Frame*>(Object object)
{
    //check if class name corresponds to type name
    VALUE klass = rb_funcall(object,rb_intern("class"),0);
    VALUE klass_name = rb_funcall(klass,rb_intern("name"),0);
    std::string name(StringValuePtr(klass_name));
    if(name != "/base/samples/frame/Frame")
        rb_raise(rb_eRuntimeError, "wrong Argument: Expected /base/samples/frame/Frame but got %s",name.c_str());

    //convert VALUE to return value
    static orogen_transports::TypelibMarshallerBase* typelib_marshaller = orogen_transports::getMarshallerFor(name.c_str());
    if(typelib_marshaller == NULL)
        rb_raise(rb_eRuntimeError, "cannot get marshaller for %s",name.c_str());
    static orogen_transports::TypelibMarshallerBase::Handle &handle = *typelib_marshaller->createHandle();

    Typelib::Value typelib_value = typelib_get(object);
    typelib_marshaller->setTypelibSample(&handle, reinterpret_cast<uint8_t*>(typelib_value.getData()), true);
    void* cxx_data = typelib_marshaller->getOrocosSample(&handle);
    return (Frame*)cxx_data;
}

template<>
std::vector<uint8_t> from_ruby<std::vector<uint8_t> >(Object obj)
{
    Rice::Array a(obj);
    std::vector<uint8_t> result;
    result.reserve(a.size());
    for (Rice::Array::iterator it = a.begin(); it != a.end(); ++it) {
        result.push_back(from_ruby<uint8_t>(*it));
    }
    return result;
}

FrameHelper sframe_helper;
void saveFrame(std::string filename,Frame& frame)
{
    sframe_helper.saveFrame(filename,frame);
}
void loadFrame(std::string filename,Frame& frame)
{
    sframe_helper.loadFrame(filename,frame);
}

void loadJpegFromData(Object obj, Frame& frame)
{
    cv::Mat mat;
    cv::imdecode(from_ruby<std::vector<uint8_t> >(obj), cv::IMREAD_COLOR, &mat);
    FrameHelper::copyMatToFrame(mat, frame);
}



extern "C"
void Init_frame_helper_ruby()
{
  Rice::Data_Type<FrameHelper> rb_cFrameHelper =
    define_class<FrameHelper>("FrameHelper")
    .define_constructor(Constructor<FrameHelper>())
    .define_singleton_method("save_frame", &saveFrame,(Arg("filename"),Arg("image")))
    .define_singleton_method("load_frame", &loadFrame,(Arg("filename"),Arg("image")))
    .define_singleton_method("load_jpeg_from_data", &loadJpegFromData, (Arg("bytes"),Arg("image")))
    .define_method("save_frame", &frame_helper::FrameHelper::saveFrame,(Arg("filename"),Arg("image")))
    .define_method("load_frame", &frame_helper::FrameHelper::loadFrame,(Arg("filename"),Arg("image")));
}
