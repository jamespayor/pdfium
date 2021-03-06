// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_FXFA_FXFA_BASIC_H_
#define XFA_FXFA_FXFA_BASIC_H_

#include "fxjs/cfxjse_arguments.h"
#include "fxjs/fxjse.h"

class CFXJSE_Arguments;
class CJX_Object;
class CXFA_Measurement;
enum class XFA_ObjectType;

enum XFA_HashCode : uint32_t {
  XFA_HASHCODE_None = 0,

  XFA_HASHCODE_Config = 0x4e1e39b6,
  XFA_HASHCODE_ConnectionSet = 0xe14c801c,
  XFA_HASHCODE_Data = 0xbde9abda,
  XFA_HASHCODE_DataDescription = 0x2b5df51e,
  XFA_HASHCODE_Datasets = 0x99b95079,
  XFA_HASHCODE_DataWindow = 0x83a550d2,
  XFA_HASHCODE_Event = 0x185e41e2,
  XFA_HASHCODE_Form = 0xcd309ff4,
  XFA_HASHCODE_Group = 0xf7f75fcd,
  XFA_HASHCODE_Host = 0xdb075bde,
  XFA_HASHCODE_Layout = 0x7e7e845e,
  XFA_HASHCODE_LocaleSet = 0x5473b6dc,
  XFA_HASHCODE_Log = 0x0b1b3d22,
  XFA_HASHCODE_Name = 0x31b19c1,
  XFA_HASHCODE_Occur = 0xf7eebe1c,
  XFA_HASHCODE_Pdf = 0xb843dba,
  XFA_HASHCODE_Record = 0x5779d65f,
  XFA_HASHCODE_Signature = 0x8b036f32,
  XFA_HASHCODE_SourceSet = 0x811929d,
  XFA_HASHCODE_Stylesheet = 0x6038580a,
  XFA_HASHCODE_Template = 0x803550fc,
  XFA_HASHCODE_This = 0x2d574d58,
  XFA_HASHCODE_Xdc = 0xc56afbf,
  XFA_HASHCODE_XDP = 0xc56afcc,
  XFA_HASHCODE_Xfa = 0xc56b9ff,
  XFA_HASHCODE_Xfdf = 0x48d004a8,
  XFA_HASHCODE_Xmpmeta = 0x132a8fbc
};

enum class XFA_PacketType : uint8_t {
  User,
  SourceSet,
  Pdf,
  Xdc,
  Xdp,
  Xmpmeta,
  Xfdf,
  Config,
  LocaleSet,
  Stylesheet,
  Template,
  Signature,
  Datasets,
  Form,
  ConnectionSet,
};

enum XFA_XDPPACKET {
  XFA_XDPPACKET_UNKNOWN = 0,
  XFA_XDPPACKET_Config = 1 << static_cast<uint8_t>(XFA_PacketType::Config),
  XFA_XDPPACKET_Template = 1 << static_cast<uint8_t>(XFA_PacketType::Template),
  XFA_XDPPACKET_Datasets = 1 << static_cast<uint8_t>(XFA_PacketType::Datasets),
  XFA_XDPPACKET_Form = 1 << static_cast<uint8_t>(XFA_PacketType::Form),
  XFA_XDPPACKET_LocaleSet = 1
                            << static_cast<uint8_t>(XFA_PacketType::LocaleSet),
  XFA_XDPPACKET_ConnectionSet =
      1 << static_cast<uint8_t>(XFA_PacketType::ConnectionSet),
  XFA_XDPPACKET_SourceSet = 1
                            << static_cast<uint8_t>(XFA_PacketType::SourceSet),
  XFA_XDPPACKET_Xdc = 1 << static_cast<uint8_t>(XFA_PacketType::Xdc),
  XFA_XDPPACKET_Pdf = 1 << static_cast<uint8_t>(XFA_PacketType::Pdf),
  XFA_XDPPACKET_Xfdf = 1 << static_cast<uint8_t>(XFA_PacketType::Xfdf),
  XFA_XDPPACKET_Xmpmeta = 1 << static_cast<uint8_t>(XFA_PacketType::Xmpmeta),
  XFA_XDPPACKET_Signature = 1
                            << static_cast<uint8_t>(XFA_PacketType::Signature),
  XFA_XDPPACKET_Stylesheet =
      1 << static_cast<uint8_t>(XFA_PacketType::Stylesheet),
  XFA_XDPPACKET_USER = 1 << static_cast<uint8_t>(XFA_PacketType::User),
  XFA_XDPPACKET_XDP = 1 << static_cast<uint8_t>(XFA_PacketType::Xdp),
};

enum XFA_XDPPACKET_FLAGS {
  XFA_XDPPACKET_FLAGS_COMPLETEMATCH = 1,
  XFA_XDPPACKET_FLAGS_PREFIXMATCH = 2,
  XFA_XDPPACKET_FLAGS_NOMATCH = 4,
  XFA_XDPPACKET_FLAGS_SUPPORTONE = 8,
  XFA_XDPPACKET_FLAGS_SUPPORTMANY = 16,
};

struct XFA_PACKETINFO {
  uint32_t uHash;
  const wchar_t* pName;
  XFA_XDPPACKET eName;
  const wchar_t* pURI;
  uint32_t eFlags;
};

enum XFA_ATTRIBUTEENUM {
  XFA_ATTRIBUTEENUM_Asterisk,
  XFA_ATTRIBUTEENUM_Slash,
  XFA_ATTRIBUTEENUM_Backslash,
  XFA_ATTRIBUTEENUM_On,
  XFA_ATTRIBUTEENUM_Tb,
  XFA_ATTRIBUTEENUM_Up,
  XFA_ATTRIBUTEENUM_MetaData,
  XFA_ATTRIBUTEENUM_Delegate,
  XFA_ATTRIBUTEENUM_PostSubmit,
  XFA_ATTRIBUTEENUM_Name,
  XFA_ATTRIBUTEENUM_Cross,
  XFA_ATTRIBUTEENUM_Next,
  XFA_ATTRIBUTEENUM_None,
  XFA_ATTRIBUTEENUM_ShortEdge,
  XFA_ATTRIBUTEENUM_1mod10_1mod11,
  XFA_ATTRIBUTEENUM_Height,
  XFA_ATTRIBUTEENUM_CrossDiagonal,
  XFA_ATTRIBUTEENUM_All,
  XFA_ATTRIBUTEENUM_Any,
  XFA_ATTRIBUTEENUM_ToRight,
  XFA_ATTRIBUTEENUM_MatchTemplate,
  XFA_ATTRIBUTEENUM_Dpl,
  XFA_ATTRIBUTEENUM_Invisible,
  XFA_ATTRIBUTEENUM_Fit,
  XFA_ATTRIBUTEENUM_Width,
  XFA_ATTRIBUTEENUM_PreSubmit,
  XFA_ATTRIBUTEENUM_Ipl,
  XFA_ATTRIBUTEENUM_FlateCompress,
  XFA_ATTRIBUTEENUM_Med,
  XFA_ATTRIBUTEENUM_Odd,
  XFA_ATTRIBUTEENUM_Off,
  XFA_ATTRIBUTEENUM_Pdf,
  XFA_ATTRIBUTEENUM_Row,
  XFA_ATTRIBUTEENUM_Top,
  XFA_ATTRIBUTEENUM_Xdp,
  XFA_ATTRIBUTEENUM_Xfd,
  XFA_ATTRIBUTEENUM_Xml,
  XFA_ATTRIBUTEENUM_Zip,
  XFA_ATTRIBUTEENUM_Zpl,
  XFA_ATTRIBUTEENUM_Visible,
  XFA_ATTRIBUTEENUM_Exclude,
  XFA_ATTRIBUTEENUM_MouseEnter,
  XFA_ATTRIBUTEENUM_Pair,
  XFA_ATTRIBUTEENUM_Filter,
  XFA_ATTRIBUTEENUM_MoveLast,
  XFA_ATTRIBUTEENUM_ExportAndImport,
  XFA_ATTRIBUTEENUM_Push,
  XFA_ATTRIBUTEENUM_Portrait,
  XFA_ATTRIBUTEENUM_Default,
  XFA_ATTRIBUTEENUM_StoredProc,
  XFA_ATTRIBUTEENUM_StayBOF,
  XFA_ATTRIBUTEENUM_StayEOF,
  XFA_ATTRIBUTEENUM_PostPrint,
  XFA_ATTRIBUTEENUM_UsCarrier,
  XFA_ATTRIBUTEENUM_Right,
  XFA_ATTRIBUTEENUM_PreOpen,
  XFA_ATTRIBUTEENUM_Actual,
  XFA_ATTRIBUTEENUM_Rest,
  XFA_ATTRIBUTEENUM_TopCenter,
  XFA_ATTRIBUTEENUM_StandardSymbol,
  XFA_ATTRIBUTEENUM_Initialize,
  XFA_ATTRIBUTEENUM_JustifyAll,
  XFA_ATTRIBUTEENUM_Normal,
  XFA_ATTRIBUTEENUM_Landscape,
  XFA_ATTRIBUTEENUM_NonInteractive,
  XFA_ATTRIBUTEENUM_MouseExit,
  XFA_ATTRIBUTEENUM_Minus,
  XFA_ATTRIBUTEENUM_DiagonalLeft,
  XFA_ATTRIBUTEENUM_SimplexPaginated,
  XFA_ATTRIBUTEENUM_Document,
  XFA_ATTRIBUTEENUM_Warning,
  XFA_ATTRIBUTEENUM_Auto,
  XFA_ATTRIBUTEENUM_Below,
  XFA_ATTRIBUTEENUM_BottomLeft,
  XFA_ATTRIBUTEENUM_BottomCenter,
  XFA_ATTRIBUTEENUM_Tcpl,
  XFA_ATTRIBUTEENUM_Text,
  XFA_ATTRIBUTEENUM_Grouping,
  XFA_ATTRIBUTEENUM_SecureSymbol,
  XFA_ATTRIBUTEENUM_PreExecute,
  XFA_ATTRIBUTEENUM_DocClose,
  XFA_ATTRIBUTEENUM_Keyset,
  XFA_ATTRIBUTEENUM_Vertical,
  XFA_ATTRIBUTEENUM_PreSave,
  XFA_ATTRIBUTEENUM_PreSign,
  XFA_ATTRIBUTEENUM_Bottom,
  XFA_ATTRIBUTEENUM_ToTop,
  XFA_ATTRIBUTEENUM_Verify,
  XFA_ATTRIBUTEENUM_First,
  XFA_ATTRIBUTEENUM_ContentArea,
  XFA_ATTRIBUTEENUM_Solid,
  XFA_ATTRIBUTEENUM_Pessimistic,
  XFA_ATTRIBUTEENUM_DuplexPaginated,
  XFA_ATTRIBUTEENUM_Round,
  XFA_ATTRIBUTEENUM_Remerge,
  XFA_ATTRIBUTEENUM_Ordered,
  XFA_ATTRIBUTEENUM_Percent,
  XFA_ATTRIBUTEENUM_Even,
  XFA_ATTRIBUTEENUM_Exit,
  XFA_ATTRIBUTEENUM_ToolTip,
  XFA_ATTRIBUTEENUM_OrderedOccurrence,
  XFA_ATTRIBUTEENUM_ReadOnly,
  XFA_ATTRIBUTEENUM_Currency,
  XFA_ATTRIBUTEENUM_Concat,
  XFA_ATTRIBUTEENUM_Thai,
  XFA_ATTRIBUTEENUM_Embossed,
  XFA_ATTRIBUTEENUM_Formdata,
  XFA_ATTRIBUTEENUM_Greek,
  XFA_ATTRIBUTEENUM_Decimal,
  XFA_ATTRIBUTEENUM_Select,
  XFA_ATTRIBUTEENUM_LongEdge,
  XFA_ATTRIBUTEENUM_Protected,
  XFA_ATTRIBUTEENUM_BottomRight,
  XFA_ATTRIBUTEENUM_Zero,
  XFA_ATTRIBUTEENUM_ForwardOnly,
  XFA_ATTRIBUTEENUM_DocReady,
  XFA_ATTRIBUTEENUM_Hidden,
  XFA_ATTRIBUTEENUM_Include,
  XFA_ATTRIBUTEENUM_Dashed,
  XFA_ATTRIBUTEENUM_MultiSelect,
  XFA_ATTRIBUTEENUM_Inactive,
  XFA_ATTRIBUTEENUM_Embed,
  XFA_ATTRIBUTEENUM_Static,
  XFA_ATTRIBUTEENUM_OnEntry,
  XFA_ATTRIBUTEENUM_Cyrillic,
  XFA_ATTRIBUTEENUM_NonBlank,
  XFA_ATTRIBUTEENUM_TopRight,
  XFA_ATTRIBUTEENUM_Hebrew,
  XFA_ATTRIBUTEENUM_TopLeft,
  XFA_ATTRIBUTEENUM_Center,
  XFA_ATTRIBUTEENUM_MoveFirst,
  XFA_ATTRIBUTEENUM_Diamond,
  XFA_ATTRIBUTEENUM_PageOdd,
  XFA_ATTRIBUTEENUM_1mod10,
  XFA_ATTRIBUTEENUM_Korean,
  XFA_ATTRIBUTEENUM_AboveEmbedded,
  XFA_ATTRIBUTEENUM_ZipCompress,
  XFA_ATTRIBUTEENUM_Numeric,
  XFA_ATTRIBUTEENUM_Circle,
  XFA_ATTRIBUTEENUM_ToBottom,
  XFA_ATTRIBUTEENUM_Inverted,
  XFA_ATTRIBUTEENUM_Update,
  XFA_ATTRIBUTEENUM_Isoname,
  XFA_ATTRIBUTEENUM_Server,
  XFA_ATTRIBUTEENUM_Position,
  XFA_ATTRIBUTEENUM_MiddleCenter,
  XFA_ATTRIBUTEENUM_Optional,
  XFA_ATTRIBUTEENUM_UsePrinterSetting,
  XFA_ATTRIBUTEENUM_Outline,
  XFA_ATTRIBUTEENUM_IndexChange,
  XFA_ATTRIBUTEENUM_Change,
  XFA_ATTRIBUTEENUM_PageArea,
  XFA_ATTRIBUTEENUM_Once,
  XFA_ATTRIBUTEENUM_Only,
  XFA_ATTRIBUTEENUM_Open,
  XFA_ATTRIBUTEENUM_Caption,
  XFA_ATTRIBUTEENUM_Raised,
  XFA_ATTRIBUTEENUM_Justify,
  XFA_ATTRIBUTEENUM_RefAndDescendants,
  XFA_ATTRIBUTEENUM_Short,
  XFA_ATTRIBUTEENUM_PageFront,
  XFA_ATTRIBUTEENUM_Monospace,
  XFA_ATTRIBUTEENUM_Middle,
  XFA_ATTRIBUTEENUM_PrePrint,
  XFA_ATTRIBUTEENUM_Always,
  XFA_ATTRIBUTEENUM_Unknown,
  XFA_ATTRIBUTEENUM_ToLeft,
  XFA_ATTRIBUTEENUM_Above,
  XFA_ATTRIBUTEENUM_DashDot,
  XFA_ATTRIBUTEENUM_Gregorian,
  XFA_ATTRIBUTEENUM_Roman,
  XFA_ATTRIBUTEENUM_MouseDown,
  XFA_ATTRIBUTEENUM_Symbol,
  XFA_ATTRIBUTEENUM_PageEven,
  XFA_ATTRIBUTEENUM_Sign,
  XFA_ATTRIBUTEENUM_AddNew,
  XFA_ATTRIBUTEENUM_Star,
  XFA_ATTRIBUTEENUM_Optimistic,
  XFA_ATTRIBUTEENUM_Rl_tb,
  XFA_ATTRIBUTEENUM_MiddleRight,
  XFA_ATTRIBUTEENUM_Maintain,
  XFA_ATTRIBUTEENUM_Package,
  XFA_ATTRIBUTEENUM_SimplifiedChinese,
  XFA_ATTRIBUTEENUM_ToCenter,
  XFA_ATTRIBUTEENUM_Back,
  XFA_ATTRIBUTEENUM_Unspecified,
  XFA_ATTRIBUTEENUM_BatchOptimistic,
  XFA_ATTRIBUTEENUM_Bold,
  XFA_ATTRIBUTEENUM_Both,
  XFA_ATTRIBUTEENUM_Butt,
  XFA_ATTRIBUTEENUM_Client,
  XFA_ATTRIBUTEENUM_2mod10,
  XFA_ATTRIBUTEENUM_ImageOnly,
  XFA_ATTRIBUTEENUM_Horizontal,
  XFA_ATTRIBUTEENUM_Dotted,
  XFA_ATTRIBUTEENUM_UserControl,
  XFA_ATTRIBUTEENUM_DiagonalRight,
  XFA_ATTRIBUTEENUM_ConsumeData,
  XFA_ATTRIBUTEENUM_Check,
  XFA_ATTRIBUTEENUM_Data,
  XFA_ATTRIBUTEENUM_Down,
  XFA_ATTRIBUTEENUM_SansSerif,
  XFA_ATTRIBUTEENUM_Inline,
  XFA_ATTRIBUTEENUM_TraditionalChinese,
  XFA_ATTRIBUTEENUM_Warn,
  XFA_ATTRIBUTEENUM_RefOnly,
  XFA_ATTRIBUTEENUM_InteractiveForms,
  XFA_ATTRIBUTEENUM_Word,
  XFA_ATTRIBUTEENUM_Unordered,
  XFA_ATTRIBUTEENUM_Required,
  XFA_ATTRIBUTEENUM_ImportOnly,
  XFA_ATTRIBUTEENUM_BelowEmbedded,
  XFA_ATTRIBUTEENUM_Japanese,
  XFA_ATTRIBUTEENUM_Full,
  XFA_ATTRIBUTEENUM_Rl_row,
  XFA_ATTRIBUTEENUM_Vietnamese,
  XFA_ATTRIBUTEENUM_EastEuropeanRoman,
  XFA_ATTRIBUTEENUM_MouseUp,
  XFA_ATTRIBUTEENUM_ExportOnly,
  XFA_ATTRIBUTEENUM_Clear,
  XFA_ATTRIBUTEENUM_Click,
  XFA_ATTRIBUTEENUM_Base64,
  XFA_ATTRIBUTEENUM_Close,
  XFA_ATTRIBUTEENUM_Host,
  XFA_ATTRIBUTEENUM_Global,
  XFA_ATTRIBUTEENUM_Blank,
  XFA_ATTRIBUTEENUM_Table,
  XFA_ATTRIBUTEENUM_Import,
  XFA_ATTRIBUTEENUM_Custom,
  XFA_ATTRIBUTEENUM_MiddleLeft,
  XFA_ATTRIBUTEENUM_PostExecute,
  XFA_ATTRIBUTEENUM_Radix,
  XFA_ATTRIBUTEENUM_PostOpen,
  XFA_ATTRIBUTEENUM_Enter,
  XFA_ATTRIBUTEENUM_Ignore,
  XFA_ATTRIBUTEENUM_Lr_tb,
  XFA_ATTRIBUTEENUM_Fantasy,
  XFA_ATTRIBUTEENUM_Italic,
  XFA_ATTRIBUTEENUM_Author,
  XFA_ATTRIBUTEENUM_ToEdge,
  XFA_ATTRIBUTEENUM_Choice,
  XFA_ATTRIBUTEENUM_Disabled,
  XFA_ATTRIBUTEENUM_CrossHatch,
  XFA_ATTRIBUTEENUM_DataRef,
  XFA_ATTRIBUTEENUM_DashDotDot,
  XFA_ATTRIBUTEENUM_Square,
  XFA_ATTRIBUTEENUM_Dynamic,
  XFA_ATTRIBUTEENUM_Manual,
  XFA_ATTRIBUTEENUM_Etched,
  XFA_ATTRIBUTEENUM_ValidationState,
  XFA_ATTRIBUTEENUM_Cursive,
  XFA_ATTRIBUTEENUM_Last,
  XFA_ATTRIBUTEENUM_Left,
  XFA_ATTRIBUTEENUM_Link,
  XFA_ATTRIBUTEENUM_Long,
  XFA_ATTRIBUTEENUM_InternationalCarrier,
  XFA_ATTRIBUTEENUM_PDF1_3,
  XFA_ATTRIBUTEENUM_PDF1_6,
  XFA_ATTRIBUTEENUM_Serif,
  XFA_ATTRIBUTEENUM_PostSave,
  XFA_ATTRIBUTEENUM_Ready,
  XFA_ATTRIBUTEENUM_PostSign,
  XFA_ATTRIBUTEENUM_Arabic,
  XFA_ATTRIBUTEENUM_Error,
  XFA_ATTRIBUTEENUM_Urlencoded,
  XFA_ATTRIBUTEENUM_Lowered,
};

enum class XFA_Attribute : uint8_t {
  H = 0,
  W,
  X,
  Y,
  Id,
  To,
  LineThrough,
  HAlign,
  Typeface,
  BeforeTarget,
  Name,
  Next,
  DataRowCount,
  Break,
  VScrollPolicy,
  FontHorizontalScale,
  TextIndent,
  Context,
  TrayOut,
  Cap,
  Max,
  Min,
  Ref,
  Rid,
  Url,
  Use,
  LeftInset,
  Widows,
  Level,
  BottomInset,
  OverflowTarget,
  AllowMacro,
  PagePosition,
  ColumnWidths,
  OverflowLeader,
  Action,
  NonRepudiation,
  Rate,
  AllowRichText,
  Role,
  OverflowTrailer,
  Operation,
  Timeout,
  TopInset,
  Access,
  CommandType,
  Format,
  DataPrep,
  WidgetData,
  Abbr,
  MarginRight,
  DataDescription,
  EncipherOnly,
  KerningMode,
  Rotate,
  WordCharacterCount,
  Type,
  Reserve,
  TextLocation,
  Priority,
  Underline,
  ModuleWidth,
  Hyphenate,
  Listen,
  Delimiter,
  ContentType,
  StartNew,
  EofAction,
  AllowNeutral,
  Connection,
  BaselineShift,
  OverlinePeriod,
  FracDigits,
  Orientation,
  TimeStamp,
  PrintCheckDigit,
  MarginLeft,
  Stroke,
  ModuleHeight,
  TransferEncoding,
  Usage,
  Presence,
  RadixOffset,
  Preserve,
  AliasNode,
  MultiLine,
  Version,
  StartChar,
  ScriptTest,
  StartAngle,
  CursorType,
  DigitalSignature,
  CodeType,
  Output,
  BookendTrailer,
  ImagingBBox,
  ExcludeInitialCap,
  Force,
  CrlSign,
  Previous,
  PushCharacterCount,
  NullTest,
  RunAt,
  SpaceBelow,
  SweepAngle,
  NumberOfCells,
  LetterSpacing,
  LockType,
  PasswordChar,
  VAlign,
  SourceBelow,
  Inverted,
  Mark,
  MaxH,
  MaxW,
  Truncate,
  MinH,
  MinW,
  Initial,
  Mode,
  Layout,
  Server,
  EmbedPDF,
  OddOrEven,
  TabDefault,
  Contains,
  RightInset,
  MaxChars,
  Open,
  Relation,
  WideNarrowRatio,
  Relevant,
  SignatureType,
  LineThroughPeriod,
  Shape,
  TabStops,
  OutputBelow,
  Short,
  FontVerticalScale,
  Thickness,
  CommitOn,
  RemainCharacterCount,
  KeyAgreement,
  ErrorCorrectionLevel,
  UpsMode,
  MergeMode,
  Circular,
  PsName,
  Trailer,
  UnicodeRange,
  ExecuteType,
  DuplexImposition,
  TrayIn,
  BindingNode,
  BofAction,
  Save,
  TargetType,
  KeyEncipherment,
  CredentialServerPolicy,
  Size,
  InitialNumber,
  Slope,
  CSpace,
  ColSpan,
  Binding,
  Checksum,
  CharEncoding,
  Bind,
  TextEntry,
  Archive,
  Uuid,
  Posture,
  After,
  Orphans,
  QualifiedName,
  Usehref,
  Locale,
  Weight,
  UnderlinePeriod,
  Data,
  Desc,
  Numbered,
  DataColumnCount,
  Overline,
  UrlPolicy,
  AnchorType,
  LabelRef,
  BookendLeader,
  MaxLength,
  AccessKey,
  CursorLocation,
  DelayedOpen,
  Target,
  DataEncipherment,
  AfterTarget,
  Leader,
  Picker,
  From,
  BaseProfile,
  Aspect,
  RowColumnRatio,
  LineHeight,
  Highlight,
  ValueRef,
  MaxEntries,
  DataLength,
  Activity,
  Input,
  Value,
  BlankOrNotBlank,
  AddRevocationInfo,
  GenericFamily,
  Hand,
  Href,
  TextEncoding,
  LeadDigits,
  Permissions,
  SpaceAbove,
  CodeBase,
  Stock,
  IsNull,
  RestoreState,
  ExcludeAllCaps,
  FormatTest,
  HScrollPolicy,
  Join,
  KeyCertSign,
  Radius,
  SourceAbove,
  Override,
  ClassId,
  Disable,
  Scope,
  Match,
  Placement,
  Before,
  WritingScript,
  EndChar,
  Lock,
  Long,
  Intact,
  XdpContent,
  DecipherOnly,

  Unknown = 255,
};

enum class XFA_Element : int32_t {
  Unknown = -1,

  Ps,
  To,
  Ui,
  RecordSet,
  SubsetBelow,
  SubformSet,
  AdobeExtensionLevel,
  Typeface,
  Break,
  FontInfo,
  NumberPattern,
  DynamicRender,
  PrintScaling,
  CheckButton,
  DatePatterns,
  SourceSet,
  Amd,
  Arc,
  Day,
  Era,
  Jog,
  Log,
  Map,
  Mdp,
  BreakBefore,
  Oid,
  Pcl,
  Pdf,
  Ref,
  Uri,
  Xdc,
  Xdp,
  Xfa,
  Xsl,
  Zpl,
  Cache,
  Margin,
  KeyUsage,
  Exclude,
  ChoiceList,
  Level,
  LabelPrinter,
  CalendarSymbols,
  Para,
  Part,
  Pdfa,
  Filter,
  Present,
  Pagination,
  Encoding,
  Event,
  Whitespace,
  DefaultUi,
  DataModel,
  Barcode,
  TimePattern,
  BatchOutput,
  Enforce,
  CurrencySymbols,
  AddSilentPrint,
  Rename,
  Operation,
  Typefaces,
  SubjectDNs,
  Issuers,
  SignaturePseudoModel,
  WsdlConnection,
  Debug,
  Delta,
  EraNames,
  ModifyAnnots,
  StartNode,
  Button,
  Format,
  Border,
  Area,
  Hyphenation,
  Text,
  Time,
  Type,
  Overprint,
  Certificates,
  EncryptionMethods,
  SetProperty,
  PrinterName,
  StartPage,
  PageOffset,
  DateTime,
  Comb,
  Pattern,
  IfEmpty,
  SuppressBanner,
  OutputBin,
  Field,
  Agent,
  OutputXSL,
  AdjustData,
  AutoSave,
  ContentArea,
  EventPseudoModel,
  WsdlAddress,
  Solid,
  DateTimeSymbols,
  EncryptionLevel,
  Edge,
  Stipple,
  Attributes,
  VersionControl,
  Meridiem,
  ExclGroup,
  ToolTip,
  Compress,
  Reason,
  Execute,
  ContentCopy,
  DateTimeEdit,
  Config,
  Image,
  SharpxHTML,
  NumberOfCopies,
  BehaviorOverride,
  TimeStamp,
  Month,
  ViewerPreferences,
  ScriptModel,
  Decimal,
  Subform,
  Select,
  Window,
  LocaleSet,
  Handler,
  HostPseudoModel,
  Presence,
  Record,
  Embed,
  Version,
  Command,
  Copies,
  Staple,
  SubmitFormat,
  Boolean,
  Message,
  Output,
  PsMap,
  ExcludeNS,
  Assist,
  Picture,
  Traversal,
  SilentPrint,
  WebClient,
  LayoutPseudoModel,
  Producer,
  Corner,
  MsgId,
  Color,
  Keep,
  Query,
  Insert,
  ImageEdit,
  Validate,
  DigestMethods,
  NumberPatterns,
  PageSet,
  Integer,
  SoapAddress,
  Equate,
  FormFieldFilling,
  PageRange,
  Update,
  ConnectString,
  Mode,
  Layout,
  Sharpxml,
  XsdConnection,
  Traverse,
  Encodings,
  Template,
  Acrobat,
  ValidationMessaging,
  Signing,
  DataWindow,
  Script,
  AddViewerPreferences,
  AlwaysEmbed,
  PasswordEdit,
  NumericEdit,
  EncryptionMethod,
  Change,
  PageArea,
  SubmitUrl,
  Oids,
  Signature,
  ADBE_JSConsole,
  Caption,
  Relevant,
  FlipLabel,
  ExData,
  DayNames,
  SoapAction,
  DefaultTypeface,
  Manifest,
  Overflow,
  Linear,
  CurrencySymbol,
  Delete,
  Deltas,
  DigestMethod,
  InstanceManager,
  EquateRange,
  Medium,
  TextEdit,
  TemplateCache,
  CompressObjectStream,
  DataValue,
  AccessibleContent,
  NodeList,
  IncludeXDPContent,
  XmlConnection,
  ValidateApprovalSignatures,
  SignData,
  Packets,
  DatePattern,
  DuplexOption,
  Base,
  Bind,
  Compression,
  User,
  Rectangle,
  EffectiveOutputPolicy,
  ADBE_JSDebugger,
  Acrobat7,
  Interactive,
  Locale,
  CurrentPage,
  Data,
  Date,
  Desc,
  Encrypt,
  Draw,
  Encryption,
  MeridiemNames,
  Messaging,
  Speak,
  DataGroup,
  Common,
  Sharptext,
  PaginationOverride,
  Reasons,
  SignatureProperties,
  Threshold,
  AppearanceFilter,
  Fill,
  Font,
  Form,
  MediumInfo,
  Certificate,
  Password,
  RunScripts,
  Trace,
  Float,
  RenderPolicy,
  LogPseudoModel,
  Destination,
  Value,
  Bookend,
  ExObject,
  OpenAction,
  NeverEmbed,
  BindItems,
  Calculate,
  Print,
  Extras,
  Proto,
  DSigData,
  Creator,
  Connect,
  Permissions,
  ConnectionSet,
  Submit,
  Range,
  Linearized,
  Packet,
  RootElement,
  PlaintextMetadata,
  NumberSymbols,
  PrintHighQuality,
  Driver,
  IncrementalLoad,
  SubjectDN,
  CompressLogicalStructure,
  IncrementalMerge,
  Radial,
  Variables,
  TimePatterns,
  EffectiveInputPolicy,
  NameAttr,
  Conformance,
  Transform,
  LockDocument,
  BreakAfter,
  Line,
  List,
  Source,
  Occur,
  PickTrayByPDFSize,
  MonthNames,
  Severity,
  GroupParent,
  DocumentAssembly,
  NumberSymbol,
  Tagged,
  Items
};

struct XFA_ELEMENTINFO {
  uint32_t uHash;
  const wchar_t* pName;
  XFA_Element eName;
  uint32_t dwPackets;
  XFA_ObjectType eObjectType;
};

enum class XFA_AttributeType : uint8_t {
  NotSure,
  Enum,
  CData,
  Boolean,
  Integer,
  Measure,
};

struct XFA_ATTRIBUTEINFO {
  uint32_t uHash;
  const wchar_t* pName;
  XFA_Attribute eName;
  XFA_AttributeType eType;
  uint32_t dwPackets;
  void* pDefValue;
};

struct XFA_ELEMENTHIERARCHY {
  uint16_t wStart;
  uint16_t wCount;
};

struct XFA_SCRIPTHIERARCHY {
  uint16_t wMethodStart;
  uint16_t wMethodCount;
  uint16_t wAttributeStart;
  uint16_t wAttributeCount;
  int16_t wParentIndex;
};

#define XFA_PROPERTYFLAG_OneOf 0x01
#define XFA_PROPERTYFLAG_DefaultOneOf 0x02
struct XFA_PROPERTY {
  XFA_Element eName;
  uint8_t uOccur;
  uint8_t uFlags;
};

struct XFA_ATTRIBUTEENUMINFO {
  uint32_t uHash;
  const wchar_t* pName;
  XFA_ATTRIBUTEENUM eName;
};

enum class XFA_Unit : uint8_t {
  Percent = 0,
  Angle,
  Em,
  Pt,
  In,
  Pc,
  Cm,
  Mm,
  Mp,

  Unknown = 255,
};

struct XFA_NOTSUREATTRIBUTE {
  XFA_Element eElement;
  XFA_Attribute attribute;
  XFA_AttributeType eType;
  void* pValue;
};

typedef void (CJX_Object::*XFA_METHOD_CALLBACK)(CFXJSE_Arguments* pArguments);

struct XFA_METHODINFO {
  uint32_t uHash;
  const wchar_t* pName;
  XFA_METHOD_CALLBACK callback;
};

typedef void (CJX_Object::*XFA_ATTRIBUTE_CALLBACK)(CFXJSE_Value* pValue,
                                                   bool bSetting,
                                                   XFA_Attribute eAttribute);
enum class XFA_ScriptType : uint8_t {
  Basic,
  Object,
};

struct XFA_SCRIPTATTRIBUTEINFO {
  uint32_t uHash;
  const wchar_t* pName;
  XFA_ATTRIBUTE_CALLBACK callback;
  XFA_Attribute attribute;
  XFA_ScriptType eValueType;
};

#endif  // XFA_FXFA_FXFA_BASIC_H_
