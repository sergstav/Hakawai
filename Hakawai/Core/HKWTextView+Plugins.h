//
//  HKWTextView+Plugins.h
//  Hakawai
//
//  Copyright (c) 2014 LinkedIn
//  Released under the terms of the MIT License
//

#import "HKWTextView.h"

/*!
 An enum describing a version of the single line viewport mode supported by the rich text editor.

 \c HKWViewportModeTop locks the viewport to the top line of text.

 \c HKWViewportModeBottom locks the viewport to the bottom line of text.
 */
typedef enum {
    HKWViewportModeTop = 0,
    HKWViewportModeBottom
} HKWViewportMode;

/*!
 An enum describing the way the accessory view should be added to the parent view.

 \c HKWAccessoryViewModeSibling causes the accessory view to be attached to the text view's superview, so the text view
 and accessory view are 'siblings' in the view hierarchy. This mode is intended for use cases where the text view
 resides within some container that should also hold the accessory views.

 \c HKWAccessoryViewModeFreeFloating causes the accessory view to be attached to the 'top level' view, specified by the
 \c setTopLevelViewForAccessoryViewPositioning: method. If the top level view is not specified, the text view tries to
 figure out the top level view at the time the view is attached.
 */
typedef enum {
    HKWAccessoryViewModeSibling = 0,
    HKWAccessoryViewModeFreeFloating
} HKWAccessoryViewMode;

/*!
 This category provides an API for plug-ins registered to the text view.
 */
@interface HKWTextView (Plugins)

#pragma mark - API (text transformation)

/*!
 Replace the currently selected text with an arbitrary attributed string generated by a transformer block. If no text is
 selected, this method does nothing.

 \param transformer    a block which takes as input an attributed string corresponding to the originally selected text,
                       and outputs an attributed string which should replace the selected text
 */
- (void)transformSelectedTextWithTransformer:(NSAttributedString *(^)(NSAttributedString *))transformer;

/*!
 Given a selection range and a transformer block, replace the text within that range with an arbitrary attributed string
 generated by a transformer block.

 \param range          the range of the text within the text view to transform. If the range extends past the end of the
                       original text, it is automatically truncated. If the range begins past the end of the original
                       text, this method does nothing.
 \param transformer    a block which takes as input an attributed string corresponding to the text within the \c range
                       parameter, and outputs an attributed string which should replace the selected text
 */
- (void)transformTextAtRange:(NSRange)range
             withTransformer:(NSAttributedString *(^)(NSAttributedString *))transformer;

/*!
 Insert plain text at an index location within the text view's attributed text. The text is formatted with the default
 attributes contained within the \c typingAttributes dictionary
 */
- (void)insertPlainText:(NSString *)text location:(NSUInteger)location;

/*!
 Insert attributed text at an index location within the text view's attributed text.
 */
- (void)insertAttributedText:(NSAttributedString *)text location:(NSUInteger)location;

/*!
 Insert a text attachment at a location within the text view's attributed text.
 */
- (void)insertTextAttachment:(NSTextAttachment *)attachment location:(NSUInteger)location;

/*!
 Excise text from the text view's attributed text, within a given range.
 */
- (void)removeTextForRange:(NSRange)range;


#pragma mark - API (attributes transformation)

/*!
 Add an attribute to be added to all subsequent text inserted.
 */
- (void)activateCustomAttributeWithName:(NSString *)name value:(id)value;

/*!
 Remove a previously added attribute, preventing it from being applied to any more inserted text.
 */
- (void)deactivateCustomAttributeWithName:(NSString *)name;

/*!
 Remove all previously added attributes.
 */
- (void)deactivateAllCustomAttributes;

/*!
 Remove an attribute with a given name from text within a given range.
 */
- (void)stripAttributeFromTextAtRange:(NSRange)range attributeName:(NSString *)attributeName;

/*!
 Replace the current value of the text view's \c typingAttributes dictionary with a new attributes dictionary generated
 by a transformer block.
 */
- (void)transformTypingAttributesWithTransformer:(NSDictionary *(^)(NSDictionary *currentAttributes))transformer;


#pragma mark - API (viewport)

/*!
 Order the text view to enter the 'single line viewport mode', where the current line is fixed in a given position and
 scrolling is disabled. If the text view is already in this mode, this method does nothing.

 \param captureTouches    if YES, taps on the text view while in single line viewport mode will not be forwarded to the
                          text view; instead they will trigger special events (that the plug-in can respond to)

 \returns a \c CGRect describing (relative to the editor view's bounds) the rectangle occupied by the single line
          viewport
 */
- (CGRect)enterSingleLineViewportMode:(HKWViewportMode)mode captureTouches:(BOOL)shouldCaptureTouches;

/*!
 Order the text view to exit the 'single line viewport mode'. This restores the user's position and the appearance of
 the text view. If the text view was not already in this mode, this method does nothing.
 */
- (void)exitSingleLineViewportMode;

/*!
 Return a rect describing the bounds of the single line viewport if the text view were ordered to enter the single line
 mode with the current selection (by calling the \c enterSingleLineViewportMode method).
 */
- (CGRect)rectForSingleLineViewportInMode:(HKWViewportMode)mode;


#pragma mark - API (helper views)

/*!
 Attach an accessory view to the text editor as a sibling view. An accessory view floats 'above' the text editor view
 and intercepts touch events within its bounds. Only one accessory view can be attached at a time. Calling this method
 with an accessory view already attached is a no-op.
 */
- (void)attachSiblingAccessoryView:(UIView *)view position:(CGPoint)position;

/*!
 Attach an accessory view to the primary view of the key window's root view controller. This is suitable for 'floating'
 accessory views whose position is only loosely coupled to the position of the text view.
 */
- (void)attachFreeFloatingAccessoryView:(UIView *)view absolutePosition:(CGPoint)position;

/*!
 Detach a previously attached accessory view.

 \note If the view was not previously attached, this method does nothing.

 \warning After detachment the view's frame's origin will be relative to the origin of the text editor's superview,
 regardless of which method was used to attach the text view.
 */
- (void)detachAccessoryView:(UIView *)view;

/*!
 Allow a plug-in to set the custom top level view.
 */
- (void)setTopLevelViewForAccessoryViewPositioning:(UIView *)view;

@end
