# Invented type backlog

From `tools/drift_report.py --write`. Every type still carrying an
invented name, with whatever evidence exists for the real one. The
methods and their refusals are described in [type recovery](type-recovery.md).

169 types: 27 with a single agreed candidate,
13 with candidates that disagree, and
129 with no candidate from any method.

## One agreed candidate

Worth reading the code for first, and agreement is a lead rather than
proof. `BillRenderPair` is the standing counterexample: two layout
methods agree on `dds3MultiBillResource_tag` and both are wrong, since it
duplicates `include/billboard.h`'s `billRenderPair_t` member for member.
Methods reading the same shape can agree and still be mistaken.

| type | candidate | from | where |
|---|---|---|---|
| `BillChildPayload` | `dds3Billboard_P2A_tag3` | signature vote | src/smt3/effect/billManager.c:124 |
| `BillRenderPair` | `dds3MultiBillResource_tag` | neighbour | src/smt3/effect/effPCPMisc.c:395 |
| `EffBillboardSetParams` | `effPCPSparkleSphere_tag` | signature vote | src/smt3/effect/effPCPMisc.c:471 |
| `EffPCPBeamLargeHead` | `effPCPRingPolyExp2_tag` | signature vote | src/smt3/effect/effPCPMisc.c:533 |
| `EffPCPChargeWork` | `effPCPMudo_tag` | position, signature vote | src/smt3/effect/effPCPMisc.c:752 |
| `EffPCPCompactRectParams` | `effPCPMultiFilterExp_tag` | signature vote | src/smt3/effect/effPCPMisc.c:872 |
| `EffPCPPairedEventWork` | `effPCPRadiateTB_tag` | position, signature vote | src/smt3/effect/effPCPMisc.c:1394 |
| `EffPCPSharedTrailRef` | `effPCPBlinkMultiBlur_tag` | signature vote | src/smt3/effect/effPCPMisc.c:1447 |
| `EffPCPSpanWork` | `effPCPBreath_tag` | signature vote | src/smt3/effect/effPCPMisc.c:1462 |
| `EffPCPSpinWork` | `effPCPBufuIce_tag` | signature vote | src/smt3/effect/effPCPMisc.c:1529 |
| `EffPCPSprayWork` | `effPCPMakka_tag` | signature vote | src/smt3/effect/effPCPMisc.c:1540 |
| `EffPCPTrailParams` | `effPCPBlinkMultiBlur_tag` | signature vote | src/smt3/effect/effPCPMisc.c:1570 |
| `EffPCPTrailWork` | `effPCPMultiBlurExp_tag` | signature vote | src/smt3/effect/effPCPMisc.c:1578 |
| `EffThunderAlphaParams` | `effPCPThunderSpherePoly_t` | signature vote | src/smt3/effect/effPCPMisc.c:1158 |
| `EffThunderCellParams` | `effPCP_ThunderRadiatePoly_t` | signature vote | src/smt3/effect/effPCPThunder.c:94 |
| `PcpFlashFadingOrbitWork` | `effPCPFlashPolyRingExp2_tag` | position, signature vote | src/smt3/effect/effPCPFlash.c:330 |
| `PcpFlashOffsetRadialWork` | `effPCPFlashPolyCircle_tag` | position, signature vote | src/smt3/effect/effPCPFlash.c:390 |
| `PcpFlashOrbitArcWork` | `effPCPFlashPolyRingExp_tag` | position, signature vote | src/smt3/effect/effPCPFlash.c:419 |
| `PcpFlashRadialStripWork` | `effPCPFlashPolySphere3_tag` | position, signature vote | src/smt3/effect/effPCPFlash.c:479 |
| `PcpFlashRadialTriangleWork` | `effPCPFlashPolySphere2Exp_tag` | position, signature vote | src/smt3/effect/effPCPFlash.c:527 |
| `PcpFlashRotatingQuadWork` | `effPCPFlashPolySphereExp_tag` | position, signature vote | src/smt3/effect/effPCPFlash.c:579 |
| `PcpFlashScalingOrbitWork` | `effPCPFlashPolyRing_tag` | position, signature vote | src/smt3/effect/effPCPFlash.c:608 |
| `PcpFlashStreakParams` | `effPCPFlashPolySphere_tag` | signature vote | src/smt3/effect/effPCPFlash.c:636 |
| `PcpFlashStreakWork` | `effPCPFlashPolySphere_tag` | position, signature vote | src/smt3/effect/effPCPFlash.c:658 |
| `PcpFlashTrianglePulseWork` | `effPCPFlashPolySphere2_tag` | position, signature vote | src/smt3/effect/effPCPFlash.c:700 |
| `PcpScatterPlainInstance` | `effPCPWwSpherePoly_tag` | position, signature vote | src/smt3/effect/effPCPScatter.c:430 |
| `PcpScatterSpinWork` | `effPCPHamaCardSphere_tag` | position, signature vote | src/smt3/effect/effPCPScatter.c:612 |

## Candidates that disagree

Two methods reading the same shape can agree and both be wrong, so a
disagreement here is the useful signal, not noise.

| type | candidates | where |
|---|---|---|
| `EffBillboardSetWork` | `effPCPHouden01_tag`, `effPCPMegidora01_tag`, `effPCPMegidoraon01_tag`, `effPCPShockWave01_tag`, `effPCPSparkleSphere_tag` | src/smt3/effect/effPCPMisc.c:476 |
| `EffPCPCompactScaleParams` | `effPCPRippleBlurExpParam_t`, `effPCPRippleBlurExp_tag` | src/smt3/effect/effPCPMisc.c:892 |
| `EffPCPCompactScatterParams` | `effPCPDistortionBlurExpParam_t`, `effPCPDistortionBlurExp_tag` | src/smt3/effect/effPCPMisc.c:912 |
| `EffPCPCompactTexturedBlurParams` | `effPCPMultiBlurExp2Param_t`, `effPCPMultiBlurExp2_tag` | src/smt3/effect/effPCPMisc.c:942 |
| `EffPCPCompactWork` | `effPCPDistortionBlurExp_tag`, `effPCPRippleBlurExp_tag` | src/smt3/effect/effPCPMisc.c:973 |
| `EffPCPFadeWork` | `effPCPHouden01_tag`, `effPCPMegidora01_tag`, `effPCPMegidoraon01_tag`, `effPCPShockWave01_tag` | src/smt3/effect/effPCPMisc.c:1252 |
| `EffPCPRingWork` | `effPCPHouden01_tag`, `effPCPMegidora01_tag`, `effPCPMegidoraon01_tag`, `effPCPShockWave01_tag` | src/smt3/effect/effPCPMisc.c:1404 |
| `EffPCPSpawnOnceWork` | `Eff_SparkleSphereBillParam_t`, `effPCPHwave01_tag` | src/smt3/effect/effPCPMisc.c:1473 |
| `EffPCPThunderGroup` | `effPCPHouden01_tag`, `effPCPMZiodin01_tag`, `effPCPMZiodin02_tag`, `effPCPMegidora01_tag`, `effPCPMegidoraon01_tag` | src/smt3/effect/effPCPMisc.c:1562 |
| `EffRingWork` | `effPCPFlashPolyRingExpH_tag`, `effPCPFlashPolyRingExpParam_t` | src/smt3/effect/effPCPFlash.c:234 |
| `EffThunderParameterHead` | `effPCPThunderSpherePolyParam_t`, `effPCPThunderSpherePoly_t` | src/smt3/effect/effPCPMisc.c:1200 |
| `SdfGsTwoVertexPayload` | `sdfPrimLineElem_t`, `sdfPrimRectElem_t` | src/smt3/sdf/sdfPrim.c:134 |
| `SdfPacketPatchLink` | `sdfGrStdDrawEnv_t`, `sdfPreDrawHook_t`, `sdfPreDrawTag_t` | src/smt3/sdf/sdfDraw.c:175 |

## No candidate from any method

Nothing in the IDB or the HD corpus has this shape in a place this
type could occupy. Some of these the originals never named either.

- `AtExitEntry`, 6 members, src/smt3/code_002C81A8.c:98 (uniform member sizes)
- `AtExitList`, 2 members, src/smt3/code_002C81A8.c:107 (uniform member sizes)
- `BattleEffect`, 17 members, src/smt3/effect/effPCPMisc.c:1097 (cannot be sized here)
- `BattleGroupNode`, 16 members, src/smt3/effect/effPCPMisc.c:1784
- `BattleGroupSlot`, 5 members, src/smt3/effect/effPCPMisc.c:1679
- `BillColorWork`, 2 members, src/smt3/effect/billManager.c:115
- `BillDrawBank`, 2 members, src/smt3/effect/billManager.c:64
- `BillGeneralEntry`, 2 members, src/smt3/effect/billManager.c:27
- `BillGeneralRow`, 3 members, src/smt3/effect/billManager.c:32
- `BillKindFuncs`, 0 members, src/smt3/effect/billManager.c:16
- `BillLinkedValue`, 2 members, src/smt3/effect/billManager.c:207 (uniform member sizes)
- `BillObj`, 16 members, src/smt3/effect/effPCPMisc.c:404
- `BillOtLink`, 1 members, src/smt3/effect/billManager.c:57 (uniform member sizes)
- `BillPacRequest`, 2 members, src/smt3/effect/billManager.c:38
- `BillPacketWork`, 5 members, src/smt3/effect/effPCPMisc.c:285
- `BillPluralRecord`, 5 members, src/smt3/effect/billManager.c:192
- `BillScreenParam`, 5 members, src/smt3/effect/billManager.c:75
- `BillTextureQuad`, 1 members, src/smt3/effect/billManager.c:120 (uniform member sizes)
- `Dds3AdminModeDispatch`, 0 members, src/smt3/code_002C81A8.c:31
- `DevRequest`, 6 members, src/smt3/effect/effPCPMisc.c:1689
- `EffBattleEntry`, 7 members, src/smt3/effect/effPCPMisc.c:1069
- `EffBattleEntryList`, 15 members, src/smt3/effect/effPCPMisc.c:1079
- `EffBattleParameterValue`, 2 members, src/smt3/effect/effPCPMisc.c:1064 (uniform member sizes)
- `EffBlurTemplateBody`, 2 members, src/smt3/effect/effPCPMisc.c:500
- `EffNode`, 4 members, src/smt3/basic/dds3EffectObjectBasic.c:135 (uniform member sizes)
- `EffOwnerBillParamView`, 2 members, src/smt3/basic/dds3EffectObjectBasic.c:212
- `EffOwnerBillPayloadView`, 3 members, src/smt3/basic/dds3EffectObjectBasic.c:185
- `EffPCPBlockSet`, 7 members, src/smt3/effect/effPCPMisc.c:680
- `EffPCPCompactTimelineParams`, 7 members, src/smt3/effect/effPCPMisc.c:849
- `EffPCPDriftEventRow`, 6 members, src/smt3/effect/effPCPMisc.c:1024 (uniform member sizes)
- `EffPCPEventPlace`, 2 members, src/smt3/effect/effPCPMisc.c:1152
- `EffPCPPairedEventRow`, 8 members, src/smt3/effect/effPCPMisc.c:1370 (uniform member sizes)
- `EffPCPRectBounds`, 4 members, src/smt3/effect/effPCPMisc.c:859 (uniform member sizes)
- `EffPCPRectParams`, 6 members, src/smt3/effect/effPCPMisc.c:863
- `EffPCPSharedTrail`, 11 members, src/smt3/effect/effPCPMisc.c:1433
- `EffPCPSpawnRangeEventRow`, 8 members, src/smt3/effect/effPCPMisc.c:1481 (uniform member sizes)
- `EffParamTable`, 2 members, src/smt3/effect/effPCPMisc.c:2355 (uniform member sizes)
- `EffParamTableRecord`, 4 members, src/smt3/effect/effPCPMisc.c:2348 (uniform member sizes)
- `EffRandState`, 1 members, src/smt3/effect/effPCPFlash.c:107 (uniform member sizes)
- `EffResourceRectDrawParams`, 2 members, src/smt3/effect/effPCPMisc.c:776
- `EffResourceRectParams`, 3 members, src/smt3/effect/effPCPMisc.c:784
- `EffResourceRectWork`, 2 members, src/smt3/effect/effPCPMisc.c:791
- `EffRingColorSlot`, 1 members, src/smt3/effect/effPCPFlash.c:221 (uniform member sizes)
- `EffRingParticle`, 4 members, src/smt3/effect/effPCPFlash.c:225 (uniform member sizes)
- `EffSpawnParams`, 11 members, src/smt3/effect/effPCPMisc.c:1626
- `EffThunderFrag`, 3 members, src/smt3/effect/effPCPThunder.c:242 (uniform member sizes)
- `EffThunderSparkParams`, 29 members, src/smt3/effect/effPCPThunder.c:313
- `EffThunderSparkWork`, 4 members, src/smt3/effect/effPCPThunder.c:346
- `EffThunderVectorParams`, 16 members, src/smt3/effect/effPCPMisc.c:1179
- `EffTrackPolyData`, 10 members, src/smt3/effect/effPCPMisc.c:1931 (uniform member sizes)
- `EffTrackPolyWork`, 3 members, src/smt3/effect/effPCPMisc.c:1958
- `EffectObjectData`, 16 members, src/smt3/basic/dds3EffectObjectBasic.c:193 (uniform member sizes)
- `IpuDmaState`, 9 members, src/smt3/effect/effPCPMisc.c:1967 (uniform member sizes)
- `MdlCtx`, 10 members, src/smt3/effect/effPCPMisc.c:2195
- `MdlDevList`, 8 members, src/smt3/effect/effPCPMisc.c:1915
- `MdlDevSlot`, 3 members, src/smt3/effect/effPCPMisc.c:1908 (uniform member sizes)
- `MdlObj`, 8 members, src/smt3/effect/effPCPMisc.c:2132
- `MdlObjectAttachment`, 5 members, src/smt3/effect/effPCPMisc.c:2143
- `MdlRecord`, 2 members, src/smt3/effect/effPCPMisc.c:1701 (uniform member sizes)
- `MdlResourceItem`, 3 members, src/smt3/effect/effPCPMisc.c:2174
- `MotionEntry`, 1 members, src/smt3/effect/effPCPMisc.c:1648 (uniform member sizes)
- `MotionTable`, 4 members, src/smt3/effect/effPCPMisc.c:1672
- `MovLinearStream`, 5 members, src/smt3/effect/effPCPMisc.c:2017 (uniform member sizes)
- `MovObjStream`, 2 members, src/smt3/effect/effPCPMisc.c:2050 (uniform member sizes)
- `MovOpenParam`, 9 members, src/smt3/sdf/sdfMovie.c:77
- `MovPacStream`, 18 members, src/smt3/effect/effPCPMisc.c:2029
- `ParSystem`, 12 members, src/smt3/effect/effPCPMisc.c:1226
- `PcpFlashColorSlot`, 3 members, src/smt3/effect/effPCPFlash.c:312 (uniform member sizes)
- `PcpFlashMotionParticle`, 3 members, src/smt3/effect/effPCPFlash.c:362 (uniform member sizes)
- `PcpFlashOrbitParticle`, 5 members, src/smt3/effect/effPCPFlash.c:322 (uniform member sizes)
- `PcpFlashPulseParticle`, 4 members, src/smt3/effect/effPCPFlash.c:453 (uniform member sizes)
- `PcpFlashQuadColorSlot`, 1 members, src/smt3/effect/effPCPFlash.c:460 (uniform member sizes)
- `PcpFlashRadialStripParticle`, 8 members, src/smt3/effect/effPCPFlash.c:466 (uniform member sizes)
- `PcpFlashRotatingParticle`, 9 members, src/smt3/effect/effPCPFlash.c:552
- `PcpFlashRotatingQuadParticle`, 8 members, src/smt3/effect/effPCPFlash.c:566 (uniform member sizes)
- `PcpScatterParamsA`, 30 members, src/smt3/effect/effPCPScatter.c:236
- `PcpScatterParamsB`, 31 members, src/smt3/effect/effPCPScatter.c:295
- `PcpScatterParamsC`, 33 members, src/smt3/effect/effPCPScatter.c:341
- `PcpScatterParticle`, 10 members, src/smt3/effect/effPCPScatter.c:270 (uniform member sizes)
- `PcpScatterPlainParams`, 25 members, src/smt3/effect/effPCPScatter.c:389
- `PcpScatterPlainParticle`, 8 members, src/smt3/effect/effPCPScatter.c:419
- `PcpScatterRadialParams`, 29 members, src/smt3/effect/effPCPScatter.c:467
- `PcpScatterRadialParticle`, 7 members, src/smt3/effect/effPCPScatter.c:500 (uniform member sizes)
- `PcpScatterRibbonParams`, 18 members, src/smt3/effect/effPCPScatter.c:537
- `PcpScatterSpinParams`, 25 members, src/smt3/effect/effPCPScatter.c:572
- `Quad`, 1 members, include/ps2asm.h:22 (uniform member sizes)
- `QuadF32`, 1 members, include/ps2asm.h:25 (uniform member sizes)
- `SdfCamera`, 0 members, src/smt3/sdf/sdf3D.c:85
- `SdfChunkHeader`, 2 members, src/smt3/effect/effPCPMisc.c:1733 (uniform member sizes)
- `SdfCommandNode`, 6 members, src/smt3/sdf/sdfCoreDraw.c:105
- `SdfDmaReferenceChainPacket`, 5 members, src/smt3/sdf/sdfDraw.c:205
- `SdfDmaTagHeader`, 12 members, src/smt3/effect/effPCPMisc.c:1838
- `SdfDrawNode`, 20 members, src/smt3/effect/effPCPMisc.c:1859
- `SdfDrawPacket`, 6 members, src/smt3/sdf/sdf2D.c:60
- `SdfDrawTransform`, 2 members, src/smt3/effect/effPCPFlash.c:116 (uniform member sizes)
- `SdfGeneratedLookupTexture`, 3 members, src/smt3/sdf/sdf3D.c:240
- `SdfGraphCopyPacket`, 4 members, src/smt3/sdf/sdfPrim.c:387 (cannot be sized here)
- `SdfGsGouraudTexturedVertex`, 3 members, src/smt3/sdf/sdfPrim.c:127 (uniform member sizes)
- `SdfGsGouraudVertex`, 2 members, src/smt3/sdf/sdfPrim.c:149 (uniform member sizes)
- `SdfGsSceneBlendRegisters`, 2 members, src/smt3/sdf/sdfDraw.c:220 (uniform member sizes)
- `SdfGsTextureStateValues`, 3 members, src/smt3/effect/effPCPFlash.c:122 (uniform member sizes)
- `SdfGsTexturedVertex`, 2 members, src/smt3/sdf/sdfPrim.c:143 (uniform member sizes)
- `SdfGsTwoVertexPacket`, 3 members, src/smt3/sdf/sdfPrim.c:234
- `SdfItem`, 12 members, src/smt3/effect/effPCPMisc.c:1745
- `SdfItemList`, 3 members, src/smt3/effect/effPCPMisc.c:1770
- `SdfItemListRef`, 4 members, src/smt3/effect/effPCPMisc.c:1776 (uniform member sizes)
- `SdfKeyPoolNode`, 4 members, src/smt3/sdf/sdfSortOt.c:52 (uniform member sizes)
- `SdfKeyTreeItem`, 3 members, src/smt3/sdf/sdfSortOt.c:29
- `SdfMotionCommand`, 1 members, src/smt3/effect/effPCPMisc.c:1659 (uniform member sizes)
- `SdfObjectRefCommand`, 11 members, src/smt3/sdf/sdfCoreDraw.c:33
- `SdfPacket`, 4 members, src/smt3/sdf/sdf_Decomp.c:43 (uniform member sizes)
- `SdfPacketSlot`, 12 members, src/smt3/sdf/sdfDraw.c:274
- `SdfPadEntry`, 16 members, src/smt3/sdf/sdf_PadManager.c:17
- `SdfPatchableResourcePacket`, 4 members, src/smt3/sdf/sdfGsMemIO.c:95 (cannot be sized here)
- `SdfPool`, 5 members, src/smt3/sdf/sdfSortOt.c:68 (uniform member sizes)
- `SdfPrimitiveRequest`, 12 members, src/smt3/sdf/sdf3DPrim.c:154
- `SdfSceneDrawPacket`, 6 members, src/smt3/sdf/sdfDraw.c:226
- `SdfStreamInputDmaTag`, 2 members, src/smt3/effect/effPCPMisc.c:2126 (uniform member sizes)
- `SdfStreamReadRequest`, 3 members, src/smt3/sdf/sdfTmx.c:106 (uniform member sizes)
- `SdfStreamState`, 6 members, src/smt3/sdf/sdfTmx.c:112 (uniform member sizes)
- `SdfTextureDrawPacket`, 7 members, src/smt3/sdf/sdf2D.c:80
- `SdfUnpacked`, 4 members, src/smt3/code_002C81A8.c:271 (uniform member sizes)
- `SdfUnpackedD`, 4 members, src/smt3/code_002C81A8.c:293 (uniform member sizes)
- `SdfVuColorTransform`, 6 members, src/smt3/sdf/sdfVu1.c:76
- `SoundBank`, 3 members, src/smt3/effect/effPCPMisc.c:1117
- `SoundMixer`, 5 members, src/smt3/effect/effPCPMisc.c:1123
- `VuBlendNode`, 12 members, src/smt3/sdf/sdfVu1.c:85
- `VuIndexedTriangleCount`, 2 members, src/smt3/sdf/sdfVu1.c:121
- `VuWork`, 31 members, src/smt3/sdf/sdfVu1.c:154
